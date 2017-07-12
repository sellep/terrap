#include "terra_schedule.h"

void terra_schedule_period_init()
{

}

BOOL terra_schedule_run_period(terra_schedule_period * const period)
{/*
	terra_scheduler_period *period;
	scheduler_period_cache *cache;
	terra_time diff;
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.period_len; i++)
	{
		period = &conf.sched_periods[i];

		if (SCHEDULER_DISABLED(period))
			continue;

		cache = CACHE(i);

		if (!terra_time_between(&runtime.now, &period->active, &period->deactive))
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
	}*/

	return TRUE;
}
