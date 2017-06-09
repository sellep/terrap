#include "terra.h"

#include <stdint.h>
#include <signal.h>

#include "conf/terra_conf.h"
#include "utils/terra_time.h"
#include "hygro/terra_hygro.h"

extern void terrad_run_clock_init(terra_conf const * const);
extern void terrad_run_clock(terra_sched_clock const * const, ssize_t const, terra_time const * const);
extern void terra_heart_beat(terra_conf const * const);

static BOOL volatile _terminate = FALSE;

void signal_handler(int signum)
{
	terra_log_info("received signal %s(%u)\n", signum == 2 ? "SIGINT" : "SIGTERM", signum);
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
	terra_hygro_res hygro_res;
	ssize_t i;
	uint64_t tick = 0;

	//initialization

	if (!register_signal_handler())
		return FALSE;

	terrad_run_clock_init(conf);

	//scheduling

	while (!_terminate)
	{
		terra_time_sys(&sys_time);

		if (tick % conf->heart_tick == 0)
		{
			terra_heart_beat(conf);
		}

		if (conf->hygro_enabled && tick % conf->hygro_tick == 0)
		{
			if (!terra_hygro_read(&hygro_res, conf))
				return FALSE;
		}

		for (i = 0 ; i < conf->sched_clocks_len; i++)
		{
			terrad_run_clock(&(conf->sched_clocks[i]), i, &sys_time);
		}

		terra_time_sleep(conf->tick);

		tick++;
	}

	return TRUE;
}
