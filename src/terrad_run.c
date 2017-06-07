#include "terra.h"

#include "conf/terra_conf.h"
#include "utils/terra_time.h"

#define SLEEP_MS 500

BOOL terrad_run(terra_conf const * const conf)
{
	terra_time sys_time;
	terra_sched_clock *clock;
	ssize_t i;
	size_t diff;

	//do we need to check here, what to switch on and what off?

	while (TRUE)
	{
		terra_time_sys(&sys_time);

		for (i = 0 ; i < conf->sched_clocks_len; i++)
		{
			clock = &conf->sched_clocks[i];

			if (!clock->sched.enabled)
				continue;

			diff = terra_time_diff(&sys_time, &clock->start);
			if (diff == 0)
			{
				terra_log_info("switch clock %s to on\n", clock->sched.name);
			}
		}

		terra_time_sleep(SLEEP_MS);
	}
}
