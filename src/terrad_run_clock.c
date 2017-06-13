#include "terra.h"

static int _terra_clock_mode[TERRA_CONF_MAX_SCHED_CLOCKS];

#define SWITCH_NOT_ON(i)(_terra_clock_mode[i] != SWITCH_ON)
#define SWITCH_NOT_OFF(i)(_terra_clock_mode[i] != SWITCH_OFF)
#define SWITCH_SET_ON(i)(_terra_clock_mode[i] = SWITCH_ON)
#define SWITCH_SET_OFF(i)(_terra_clock_mode[i] = SWITCH_OFF)

void terrad_run_clock_init(terra_conf const * const conf)
{
	ssize_t i;

	for (i = 0; i < conf->sched_clocks_len; i++)
	{
		_terra_clock_mode[i] = SWITCH_UNKNOWN;
	}
}

void terrad_run_clock(terra_sched_clock const * const clock, ssize_t const i, terra_time const * const sys_time)
{
	terra_time *start;
	terra_time *stop;
	size_t diff;
	size_t j;

	if (SCHED_DISABLED(clock))
		return;

	for (j = 0; j < clock->times_len; j++)
	{
		start = &(clock->times[j].start);
		stop = &(clock->times[j].stop);

		diff = terra_time_diff(sys_time, start);

		if (diff == 0 || terra_time_between(sys_time, start, stop))
		{
			if (SWITCH_NOT_ON(i))
			{
				SWITCH_SET_ON(i);
				terra_log_info("switch clock %s to on (diff: %us)\n", clock->sched.name, diff);
			}

			//nothing todo anymore for this schedule
			return;
		}
	}

	//here, switch should be off
	if (SWITCH_NOT_OFF(i))
	{
		SWITCH_SET_OFF(i);
		terra_log_info("switch clock %s to off\n", clock->sched.name);
	}
}
