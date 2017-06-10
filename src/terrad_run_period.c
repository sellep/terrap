#include "terra.h"

#include "conf/terra_conf.h"

typedef struct
{
	terra_time begin;
	switch_mode mode;
} sched_period_cache;

static sched_period_cache sched_period_caches[TERRA_CONF_MAX_SCHED_PERIODS];

void terrad_run_period_init(terra_conf const * const conf, terra_time const * const sys_time)
{
	ssize_t i;

	for (i = 0; i < conf->sched_periods_len; i++)
	{
		terra_time_cpy(&(sched_period_caches[i].begin), sys_time);
		sched_period_caches[i].mode = SWITCH_ON;

		//here, if enabled, we need to turn on the switches
	}	
}

BOOL terrad_run_period(terra_conf const * const conf, terra_time const * const sys_time)
{
	terra_sched_period *period;
	ssize_t i;

	for (i = 0; i < conf->sched_periods_len; i++)
	{
		period = &conf->sched_periods[i];

		if (SCHED_DISABLED(period))
			continue;

		
	}

	return TRUE;
}
