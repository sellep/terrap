#include "terra.h"

#include "conf/terra_conf.h"
#include "utils/terra_time.h"

#define SLEEP_MS 500

extern void terrad_run_clock_init();
extern void terrad_run_clock(terra_sched_clock const * const, terra_time const * const);

BOOL terrad_run(terra_conf const * const conf)
{
	terra_time sys_time;
	ssize_t i;

	//initialization
	terrad_run_clock_init();

	//scheduling

	while (TRUE)
	{
		terra_time_sys(&sys_time);

		for (i = 0 ; i < conf->sched_clocks_len; i++)
		{
			terrad_run_clock(&(conf->sched_clocks[i]));
		}

		terra_time_sleep(SLEEP_MS);
	}
}
