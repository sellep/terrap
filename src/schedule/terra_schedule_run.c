#include "terra_schedule.h"

#include "../utils/terra_signal.h"

#define DO_HEART_BEAT() if (runtime.tick % CONF_LED.heart_tick == 0) terra_heart_beat()
#define DO_HYGRO_READ() (CONF_HYGRO.enabled && !hygro_wait(&CONF_HYGRO, &NOW, runtime.hygro_err))

#define SLEEP() sleep_milliseconds(CONF_GLOBAL.delay)

//TODO: check use
static inline void mode_reinit()
{
	
}

static inline void terra_heart_beat()
{
	if (LIKELY(runtime.hygro_err == 0))
	{
		terra_led_set(CONF_LED.heart_pin, LED_ON);
		sleep_milliseconds(CONF_LED.heart_duration);
		terra_led_set(CONF_LED.heart_pin, LED_OFF);
	}
	else
	{
		terra_led_set(CONF_LED.err_pin, LED_ON);
		terra_led_set(CONF_LED.heart_pin, LED_ON);
		sleep_milliseconds(CONF_LED.heart_duration);
		terra_led_set(CONF_LED.heart_pin, LED_OFF);
		terra_led_set(CONF_LED.err_pin, LED_OFF);
	}
}

static inline BOOL schedule_run_hygro()
{
	if (UNLIKELY(DO_HYGRO_READ()))
	{
		if (LIKELY(terra_hygro_run(&RUNTIME_HUMI, &RUNTIME_TEMP, &CONF_HYGRO, &NOW, &runtime.hygro_err)))
		{
			terra_hygro_write(RUNTIME_HUMI, RUNTIME_TEMP, &NOW);
			return TRUE;
		}
	}

	return FALSE;
}

static inline void schedule_run_read_only()
{
	while (!RUNTIME_BREAK())
	{
		terra_runtime_tick();
		DO_HEART_BEAT();

		schedule_run_hygro();

		SLEEP();
	}
}

static inline void schedule_run_schedules()
{
	terra_schedule_clock *clock;
	terra_schedule_hygro *hygro;
	terra_schedule_period *period;
	terra_schedule *sched;
	size_t i;

	while (!RUNTIME_BREAK())
	{
		terra_schedule_reset();

		terra_runtime_tick();
		DO_HEART_BEAT();

		if (schedule_run_hygro())
		{
			for (i = 0; i < CONF_GLOBAL.hygro_len; i++)
			{
				hygro = SCHEDULE_GET_HYGRO(i);
				sched = SCHEDULE(hygro);

				if (SCHEDULE_ENABLED(sched) && SCHEDULE_NOT_RUN(sched))
				{
					terra_schedule_run_hygro(hygro, sched);
				}
			}
		}

		for (i = 0; i < CONF_GLOBAL.clock_len; i++)
		{
			clock = SCHEDULE_GET_CLOCK(i);
			sched = SCHEDULE(clock);

			if (SCHEDULE_ENABLED(sched) && SCHEDULE_NOT_RUN(sched))
			{
				terra_schedule_run_clock(clock, sched);
			}
		}

		for (i = 0; i < CONF_GLOBAL.period_len; i++)
		{
			period = SCHEDULE_GET_PERIOD(i);
			sched = SCHEDULE(period);

			if (SCHEDULE_ENABLED(sched) && SCHEDULE_NOT_RUN(sched))
			{
				terra_schedule_run_period(period, sched);
			}
		}

		SLEEP();
	}
}

void terra_schedule_run()
{
	BOOL reinit;

	if (!terra_signal_reg())
		goto exit;

	reinit = FALSE;

	while (TRUE)
	{
		terra_schedule_init(reinit);

		if (CONF_GLOBAL.read_only)
		{
			schedule_run_read_only();
		}
		else
		{
			schedule_run_schedules();
		}

		if (RUNTIME_TERMINATE())
			goto exit;

		terra_log_info("[terra_schedule_run] reload conf\n");
		CONF_FREE();

		if (!CONF_LOAD())
		{
			terra_log_error("[terra_schedule_run] failed to load config file\n");
			goto exit;
		}

		reinit = TRUE;
	}

exit:
	terra_led_set(CONF_LED.err_pin, LED_ON);
}
