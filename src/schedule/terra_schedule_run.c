#include "terra_schedule.h"

#include "../utils/terra_signal.h"

#define DO_HEART_BEAT() if (runtime.tick % CONF_HEART.tick == 0) terra_heart_beat()
#define DO_HYGRO_READ() (CONF_HYGRO.enabled && !hygro_wait(&CONF_HYGRO, &NOW))

#define SLEEP() sleep_milliseconds(CONF_GLOBAL.delay)

static inline void terra_heart_beat()
{
	const terra_led_cmd heart_on = LED_ON | LED_HEART;
	const terra_led_cmd heart_off = LED_OFF | LED_HEART;

	terra_led_set(CONF_HEART.pin, heart_on);
	sleep_milliseconds(CONF_HEART.duration);
	terra_led_set(CONF_HEART.pin, heart_off);
}

static inline BOOL schedule_run_hygro()
{
	if (DO_HYGRO_READ())
	{
		if (!terra_hygro_run(&RUNTIME_HUMI, &RUNTIME_TEMP, &CONF_HYGRO, &NOW))
		{
			runtime.hygro_err++;
		}
		else
		{
			runtime.hygro_err = 0;
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

static inline void schedule_init()
{
	size_t i;

	terra_pin_set_out(CONF_GLOBAL.pin_alert);
	terra_pin_set_out(CONF_HEART.pin);
	terra_pin_set_out(CONF_SWITCH.pin);

	terra_led_set(CONF_GLOBAL.pin_alert, FALSE);
	terra_led_set(CONF_HEART.pin, FALSE);

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		terra_schedule_init_clock(SCHEDULE_GET_CLOCK(i));
	}

	for (i = 0; i < CONF_GLOBAL.temp_len; i++)
	{
		terra_schedule_init_temp(SCHEDULE_GET_TEMP(i));
	}

	for (i = 0; i < CONF_GLOBAL.period_len; i++)
	{
		terra_schedule_init_period(SCHEDULE_GET_PERIOD(i));
	}
}

void terra_schedule_run()
{
	if (!terra_signal_reg())
		goto exit;

	schedule_init();

	runtime.switch_modes[0] = SWITCH_UNKNOWN;
	runtime.switch_modes[1] = SWITCH_UNKNOWN;
	runtime.switch_modes[2] = SWITCH_UNKNOWN;

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
	terra_led_set(CONF_GLOBAL.pin_alert, TRUE);
}
