#include "terra.h"

typedef struct
{
	terra_time begin;
	switch_mode mode;
} sched_period_cache;

static sched_period_cache _sched_period_caches[TERRA_CONF_MAX_SCHED_PERIODS];

static inline void change_switch(
	terra_sched_period const * const period,
	sched_period_cache * const cache,
	switch_mode const mode)
{
	terra_switch_req req;

	terra_time_cpy(&cache->begin, &runtime.now);
	cache->mode = mode;

	req.sock = period->sched.sock;
	req.set_on = mode == SWITCH_ON ? TRUE : FALSE;
	terra_switch_set(&req);
}

void terrad_run_period_init()
{
	terra_sched_period *period;
	sched_period_cache *cache;
	ssize_t i;

	for (i = 0; i < conf.sched_periods_len; i++)
	{
		period = &conf.sched_periods[i];

		if (SCHED_DISABLED(period))
			continue;

		cache = &_sched_period_caches[i];

		if (terra_time_between(&runtime.now, &period->act, &period->deact))
		{
			change_switch(period, cache, SWITCH_ON);
		}
		else
		{
			cache->mode = SWITCH_OFF;
			terra_time_cpy(&cache->begin, &runtime.now);
		}
	}
}

BOOL terrad_run_period()
{
	terra_sched_period *period;
	sched_period_cache *cache;
	terra_time diff;
	ssize_t i;

	for (i = 0; i < conf.sched_periods_len; i++)
	{
		period = &conf.sched_periods[i];

		//check scheduler disabled
		if (SCHED_DISABLED(period))
			continue;

		cache = &_sched_period_caches[i];

		//check scheduler deactive
		if (!terra_time_between(&runtime.now, &period->act, &period->deact))
		{
			if (cache->mode == SWITCH_ON)
			{
				change_switch(period, cache, SWITCH_OFF);
			}

			continue;
		}

		terra_time_difft(&diff, &runtime.now, &(cache->begin));

		if (cache->mode == SWITCH_ON)
		{
			if (terra_time_cmp(&diff, &period->on_dur) != TIME_BELOW)
			{
				change_switch(period, cache, SWITCH_OFF);
			}
		}
		else if (terra_time_cmp(&diff, &period->off_dur) != TIME_BELOW)
		{
			change_switch(period, cache, SWITCH_ON);
		}
	}

	return TRUE;
}
