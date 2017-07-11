#include "terra.h"

#include <stdint.h>
#include <signal.h>

#define DO_HEART_BEAT() (runtime.tick % CONF_HEART.tick == 0)
#define DO_HYGRO_READ() (CONF_HYGRO.enabled && !hygro_wait())

extern void terrad_run_period_init();
extern BOOL terrad_run_period();

extern void terrad_run_temp_init();
extern void terrad_run_temp(float const);

extern void terrad_run_clocks_init();

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

BOOL terrad_run()
{
	ssize_t i;

	float temp;
	float humi;

	//initialization

	if (!register_signal_handler())
		return FALSE;

	terrad_run_temp_init();
	terrad_run_period_init();
	terrad_run_clocks_init();

	//scheduling

	while (!_terminate)
	{
		//TODO: start heart

		terra_runtime_tick();

		if (DO_HEART_BEAT())
		{
			terra_heart_beat();
		}

		if (DO_HYGRO_READ())
		{
			if (!terra_hygro_run(&humi, &temp))
			{
				runtime.hygro_err++;
			}
			else
			{
				runtime.hygro_err = 0;
			}

			if(!terra_hygro_write(humi, temp))
				return FALSE;

			if (!CONF_GLOBAL.read_only)
			{
				terrad_run_temp(temp);
			}
		}

		if (!CONF_GLOBAL.read_only)
		{
			terrad_run_period();
		}

		//TODO: stop heart

		sleep_milliseconds(CONF_GLOBAL.delay);
	}

	return TRUE;
}
