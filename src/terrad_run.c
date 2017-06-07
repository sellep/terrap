#include "terra.h"

#include <signal.h>

#include "conf/terra_conf.h"
#include "utils/terra_time.h"

extern void terrad_run_clock_init(terra_conf const * const);
extern void terrad_run_clock(terra_sched_clock const * const, ssize_t const, terra_time const * const);

static BOOL volatile _terminate = FALSE;

void signal_handler(int signum)
{
	if (signum == SIGINT || signum == SIGTERM)
	{
		terra_log_info("received signal %s(%u)\n", signum == 2 ? "SIGINT" : "SIGTERM", signum);
		_terminate = TRUE;
	}
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

	//initialization

	if (!register_signal_handler())
		return FALSE;

	terrad_run_clock_init(conf);

	//scheduling

	while (!_terminate)
	{
		terra_time_sys(&sys_time);

		for (i = 0 ; i < conf->sched_clocks_len; i++)
		{
			terrad_run_clock(&(conf->sched_clocks[i]), i, &sys_time);
		}

		terra_time_sleep(conf->tick);
	}

	return TRUE;
}
