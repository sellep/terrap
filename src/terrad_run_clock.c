#include "terra.h"

/*static inline change_switch(
	terra_scheduler_clock const * const clock,
	terra_switch_mode const mode)
{
	terra_switch_req req;

	req.sock = clock->scheduler.socket;
	req.mode = mode;

	terra_switch_set(&req);

	SCHEDULER_SET_MODE(clock, mode);
}*/

void terrad_run_clocks_init()
{
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		SCHEDULE_CLOCK(i)->schedule.state = SWITCH_UNKNOWN;
	}
}

static void run_clock(terra_schedule_clock * const clock)
{
	ssize_t t;

	if (SCHEDULE_DISABLED(clock))
		return;

	/* dep check */

	for (t = 0; t < clock->time_len; t++)
	{
		if (
				terra_time_diff(&NOW, &clock->times[t].start) == 0
			||	terra_time_between(&NOW, &clock->times[t].start, &clock->times[t].stop)
		)
		{
			if (SCHEDULE_SWITCH_NOT_ON(clock))
			{
				schedule_set_switch_on(clock);
				return;
			}
		}
	}

	if (SCHEDULE_SWITCH_ON(clock))
	{
		schedule_set_switch_off(clock);
	}
}

void terrad_run_clocks()
{
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		run_clock(SCHEDULE_CLOCK(i));
	}
}
