#include "terra_schedule.h"

void terra_schedule_clock_init()
{
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		SCHEDULE_GET_CLOCK(i)->schedule.state = SWITCH_UNKNOWN;
	}
}

void terra_schedule_run_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);
	ssize_t t;

	if (!terra_schedule_depcheck(sched))
	{
		if (SCHEDULE_SWITCH_NOT_OFF(sched))
		{
			SCHEDULE_SET_SWITCH_OFF(sched);
		}

		goto end;
	}

	for (t = 0; t < clock->time_len; t++)
	{
		if (
				terra_time_diff(&NOW, &clock->times[t].start) == 0
			||	terra_time_between(&NOW, &clock->times[t].start, &clock->times[t].stop)
		)
		{
			if (SCHEDULE_SWITCH_NOT_ON(sched))
			{
				SCHEDULE_SET_SWITCH_ON(sched);
				goto end;
			}
		}
	}

	if (SCHEDULE_SWITCH_ON(sched))
	{
		SCHEDULE_SET_SWITCH_OFF(sched);
	}

end:
	sched->run = TRUE;
}