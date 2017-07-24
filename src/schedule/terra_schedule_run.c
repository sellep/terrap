#include "terra_schedule.h"

#include "../utils/terra_signal.h"

#define DO_HEART_BEAT() if (runtime.tick % CONF_LED.heart_tick == 0) terra_heart_beat()
#define DO_HYGRO_READ() (CONF_HYGRO.enabled && !hygro_wait(&CONF_HYGRO, &NOW, runtime.hygro_err))

#define SLEEP() sleep_milliseconds(CONF_GLOBAL.delay)

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
	terra_schedule_temp *temp;
	terra_schedule_period *period;
	terra_schedule *sched;
	size_t i;

	while (!RUNTIME_BREAK())
	{
		schedule_reset();

		terra_runtime_tick();
		DO_HEART_BEAT();

		if (schedule_run_hygro())
		{
			for (i = 0; i < CONF_GLOBAL.temp_len; i++)
			{
				temp = SCHEDULE_GET_TEMP(i);
				sched = SCHEDULE(temp);

				if (SCHEDULE_ENABLED(sched) && SCHEDULE_NOT_RUN(sched))
				{
					terra_schedule_run_temp(temp);
				}
			}
		}

		for (i = 0; i < CONF_GLOBAL.clock_len; i++)
		{
			clock = SCHEDULE_GET_CLOCK(i);
			sched = SCHEDULE(clock);

			if (SCHEDULE_ENABLED(sched) && SCHEDULE_NOT_RUN(sched))
			{
				terra_schedule_run_clock(clock);
			}
		}

		for (i = 0; i < CONF_GLOBAL.period_len; i++)
		{
			period = SCHEDULE_GET_PERIOD(i);
			sched = SCHEDULE(period);

			if (SCHEDULE_ENABLED(sched) && SCHEDULE_NOT_RUN(sched))
			{
				terra_schedule_run_period(period);
			}
		}

		SLEEP();
	}
}

static BOOL schedule_init()
{
	terra_schedule *sched;
	terra_schedule_clock *clock;
	terra_schedule_temp *temp;
	terra_schedule_period *period;

	size_t i;
	BOOL status = FALSE;

	terra_pin_set_out(CONF_LED.err_pin);
	terra_pin_set_out(CONF_LED.heart_pin);
	terra_pin_set_out(CONF_SWITCH.pin);

	terra_led_set(CONF_LED.heart_pin, FALSE);
	terra_led_set(CONF_LED.err_pin, FALSE);

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		clock = SCHEDULE_GET_CLOCK(i);
		sched = SCHEDULE(clock);

		if (!SCHEDULE_IN_GROUP(sched))
		{
			SCHEDULE_DISABLE(sched);
		}
		else
		{
			terra_schedule_init_clock(clock);
			if (SCHEDULE_ENABLED(sched))
			{
				status = TRUE;
			}
		}
	}

	for (i = 0; i < CONF_GLOBAL.temp_len; i++)
	{
		temp = SCHEDULE_GET_TEMP(i);
		sched = SCHEDULE(temp);

		if (!SCHEDULE_IN_GROUP(sched))
		{
			SCHEDULE_DISABLE(sched);
		}
		else
		{
			terra_schedule_init_temp(temp);
			if (SCHEDULE_ENABLED(sched))
			{
				status = TRUE;
			}
		}
	}

	for (i = 0; i < CONF_GLOBAL.period_len; i++)
	{
		period = SCHEDULE_GET_PERIOD(i);
		sched = SCHEDULE(period);

		if (!SCHEDULE_IN_GROUP(sched))
		{
			SCHEDULE_DISABLE(sched);
		}
		else
		{
			terra_schedule_init_period(period);
			if (SCHEDULE_ENABLED(sched))
			{
				status = TRUE;
			}
		}
	}

	return status;
}

void terra_schedule_run()
{
	if (!terra_signal_reg())
		goto exit;

	if (!schedule_init())
	{
		terra_log_info("[terra_schedule_run] no enabled schedule, switching to read only\n");
		CONF_GLOBAL.read_only = TRUE;
	}
	else
	{
		runtime.switch_modes[0] = SWITCH_UNKNOWN;
		runtime.switch_modes[1] = SWITCH_UNKNOWN;
		runtime.switch_modes[2] = SWITCH_UNKNOWN;
	}

	while (TRUE)
	{
		RUNTIME_RESET_RELOAD();

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
	}

exit:
	terra_led_set(CONF_LED.err_pin, LED_ON);
}
