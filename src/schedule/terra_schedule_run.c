#include "terra_schedule.h"

#include <stdint.h>
#include <signal.h>

#define DO_HEART_BEAT() if (runtime.tick % CONF_HEART.tick == 0) terra_heart_beat()
#define DO_HYGRO_READ() (CONF_HYGRO.enabled && !hygro_wait())

#define SLEEP() sleep_milliseconds(CONF_GLOBAL.delay)

extern void terra_schedule_clock_init();
extern void terra_schedule_period_init();
extern void terra_schedule_temp_init();

static BOOL volatile _terminate = FALSE;

void signal_handler(int signum)
{
#ifdef DEBUG
	terra_log_info("received signal %s(%u)\n", signum == 2 ? "SIGINT" : "SIGTERM", signum);
#endif

	_terminate = TRUE;
}

BOOL register_signal_handler()
{
	if (signal(SIGINT, signal_handler) == SIG_ERR)
	{
		terra_log_error("unable to register signal handler for SIGINT\n");
		return FALSE;
	}

	if (signal(SIGTERM, signal_handler) == SIG_ERR)
	{
		terra_log_error("unable to register signal handler for SIGTERM\n");
		return FALSE;
	}

	return TRUE;
}

static inline void terra_heart_beat()
{
	const terra_led_cmd heart_on = LED_ON | LED_HEART;
	const terra_led_cmd heart_off = LED_OFF | LED_HEART;

	terra_led_set(CONF_HEART.pin, heart_on);
	sleep_milliseconds(CONF_HEART.duration);
	terra_led_set(CONF_HEART.pin, heart_off);
}

static float _humi;
static float _temp;

static inline void schedule_run_hygro()
{
	if (DO_HYGRO_READ())
	{
		if (!terra_hygro_run(&_humi, &_temp))
		{
			runtime.hygro_err++;
		}
		else
		{
			runtime.hygro_err = 0;
		}

		terra_hygro_write(_humi, _temp);
	}
}

static inline void schedule_run_read_only()
{
	while (!_terminate)
	{
		terra_runtime_tick();
		DO_HEART_BEAT();

		schedule_run_hygro();

		SLEEP();
	}
}

static inline void schedule_init()
{
	terra_pin_set_out(CONF_GLOBAL.pin_alert);
	terra_pin_set_out(CONF_HEART.pin);
	terra_pin_set_out(CONF_SWITCH.pin);

	terra_led_set(CONF_GLOBAL.pin_alert, FALSE);
	terra_led_set(CONF_HEART.pin, FALSE);
}

static inline void schedule_run()
{
	terra_schedule *sched;
	size_t i;

	schedule_init();

	while (!_terminate)
	{
		schedule_reset();

		terra_runtime_tick();
		DO_HEART_BEAT();

		schedule_run_hygro();

		for (i = 0; i < CONF_GLOBAL.clock_len; i++)
		{
			sched = SCHEDULE(SCHEDULE_GET_CLOCK(i));

			if (SCHEDULE_DISABLED(sched))
				continue;

			if (SCHEDULE_NOT_RUN(sched))
			{
				terra_schedule_run_clock((terra_schedule_clock*) sched);
			}
		}

		for (i = 0; i < CONF_GLOBAL.temp_len; i++)
		{
			sched = SCHEDULE(SCHEDULE_GET_TEMP(i));

			if (SCHEDULE_DISABLED(sched))
				continue;

			if (SCHEDULE_NOT_RUN(sched))
			{
				terra_schedule_run_temp((terra_schedule_temp*) sched);
			}
		}

		SLEEP();
	}
}

BOOL terra_schedule_run()
{
	if (!register_signal_handler())
		return FALSE;

	terra_schedule_clock_init();
	terra_schedule_period_init();
	terra_schedule_temp_init();

	if (CONF_GLOBAL.read_only)
	{
		schedule_run_read_only();
	}
	else
	{
		schedule_run();
	}

	return TRUE;
}
