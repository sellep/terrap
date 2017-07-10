#include "terra.h"

#include <stdint.h>
#include <signal.h>

#define DO_HEART_BEAT() (runtime.tick % conf.heart_tick == 0)
#define DO_HYGRO_READ() (conf.hygro_enabled && !hygro_wait())

extern void terrad_run_period_init();
extern BOOL terrad_run_period();

extern void terrad_run_temp_init();
extern void terrad_run_temp(float const);

extern void terrad_run_clock_init();
extern void terrad_run_clock(terra_sched_clock const * const, ssize_t const);

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

	terra_led_set(conf.led_pin_heart, heart_on);
	sleep_milliseconds(conf.heart_dur);
	terra_led_set(conf.led_pin_heart, heart_off);
}

BOOL terrad_run()
{
	ssize_t i;

	float temp;
	float humi;

	//initialization

	if (!register_signal_handler())
		return FALSE;

	terra_runtime_init(TERRA_CONF_PATH);

	terrad_run_temp_init();
	terrad_run_period_init();
	terrad_run_clock_init();

	//scheduling

	while (!_terminate)
	{
		terra_runtime_tick();

		if (DO_HEART_BEAT())
		{
			terra_heart_beat();
		}

		if (DO_HYGRO_READ())
		{
			if (!terra_hygro_read_rep(&humi, &temp))
				return FALSE;

			if(!terra_hygro_write(humi, temp))
				return FALSE;

			if (!conf.read_only)
			{
				terrad_run_temp(temp);
			}
		}

		if (!conf.read_only)
		{
			terrad_run_period();

			for (i = 0 ; i < conf.sched_clocks_len; i++)
			{
				terrad_run_clock(&conf.sched_clocks[i], i);
			}
		}

		sleep_milliseconds(conf.tick);
	}

	return TRUE;
}
