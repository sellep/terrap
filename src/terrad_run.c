#include "terra.h"

#include <stdint.h>
#include <signal.h>

#define DO_HYGRO_READ(conf, tick) (conf->hygro_enabled && tick % conf->hygro_tick == 0)

extern void terra_heart_beat(terra_conf const * const);

extern void terrad_run_period_init(terra_conf const * const, terra_time const * const);
extern BOOL terrad_run_period(terra_conf const * const, terra_time const * const);

extern void terrad_run_temp_init(terra_conf const * const);
extern void terrad_run_temp(terra_conf const * const, float const);

extern void terrad_run_clock_init(terra_conf const * const);
extern void terrad_run_clock(terra_sched_clock const * const, ssize_t const, terra_time const * const);

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

BOOL terrad_run(terra_conf const * const conf)
{
	terra_time sys_time;
	ssize_t i;
	uint64_t tick = 0;

	float temp;
	float humi;

	//initialization

	if (!register_signal_handler())
		return FALSE;

	terra_time_sys(&sys_time);

	terrad_run_temp_init(conf);
	terrad_run_period_init(conf, &sys_time);
	terrad_run_clock_init(conf);

	//scheduling

	while (!_terminate)
	{
		terra_time_sys(&sys_time);

		if (tick % conf->heart_tick == 0)
		{
			terra_heart_beat(conf);
		}

		if (DO_HYGRO_READ(conf, tick))
		{
			if (!terra_hygro_read_rep(conf, &humi, &temp))
				return FALSE;

			if(!terra_hygro_write(conf, &sys_time, humi, temp))
				return FALSE;

			terrad_run_temp(conf, temp);
		}

		terrad_run_period(conf, &sys_time);

		for (i = 0 ; i < conf->sched_clocks_len; i++)
		{
			terrad_run_clock(&(conf->sched_clocks[i]), i, &sys_time);
		}

		sleep_milliseconds(conf->tick);

		tick++;
	}

	return TRUE;
}
