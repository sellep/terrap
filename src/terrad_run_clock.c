#include "terra.h"

void terrad_run_clock_init()
{
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		SCHEDULE_CLOCK(i)->schedule.state = SWITCH_UNKNOWN;
	}
}

void run_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);
	ssize_t t;

	if (SCHEDULE_DISABLED(sched))
		return;

	/* dep check */

	for (t = 0; t < clock->time_len; t++)
	{
		if (
				terra_time_diff(&NOW, &clock->times[t].start) == 0
			||	terra_time_between(&NOW, &clock->times[t].start, &clock->times[t].stop)
		)
		{
			if (SCHEDULE_SWITCH_NOT_ON(sched))
			{
				terra_switch_on(sched->socket);
				SCHEDULE_SWITCH_ON(sched);
				return;
			}
		}
	}

	if (SCHEDULE_SWITCH_ON(sched))
	{
		terra_switch_off(sched->socket);
		SCHEDULE_SWITCH_OFF(sched);
	}
}