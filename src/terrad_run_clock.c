#include "terra.h"

static inline change_switch(
	terra_scheduler_clock const * const clock,
	terra_switch_mode const mode)
{
	terra_switch_req req;

	req.sock = clock->scheduler.socket;
	req.mode = mode;

	terra_switch_set(&req);

	SCHEDULER_SET_MODE(clock, mode);
}

void terrad_run_clock_init()
{
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		_terra_clock_on[i] = -1;
	}
}

void terrad_run_clock(terra_scheduler_clock const * const clock)
{
	terra_time *start;
	terra_time *stop;
	size_t diff;
	size_t j;

	if (SCHEDULER_DISABLED(clock))
		return;

	//TODO: check dependency
	if (!terra_scheduler_deps(clock))
		return;

	for (j = 0; j < clock->time_len; j++)
	{
		start = &(clock->times[j].start);
		stop = &(clock->times[j].stop);

		diff = terra_time_diff(sys_time, start);

		if (diff == 0 || terra_time_between(&runtime.now, start, stop))
		{
			if (SWITCH_NOT_ON(clock))
			{
				change_switch(clock, SWITCH_ON);
				terra_log_info("switch clock %s to on (diff: %us)\n", clock->scheduler.name, diff);
			}

			return;
		}
	}

	//here, switch should be off
	if (SWITCH_NOT_OFF(i))
	{
		SWITCH_SET_OFF(i);
		terra_log_info("switch clock %s to off\n", clock->scheduler.name);
	}
}
