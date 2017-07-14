#include "terra_schedule.h"

void terra_schedule_run_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);
	ssize_t t;

	if (!terra_schedule_depcheck(sched))
	{
		if (SWITCH_NOT_OFF(sched->socket))
		{
			SWITCH_SET(sched->socket, SWITCH_OFF);
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
			if (SWITCH_NOT_ON(sched->socket))
			{
				SWITCH_SET(sched->socket, SWITCH_ON);
			}

			goto end;
		}
	}

	if (SWITCH_NOT_OFF(sched->socket))
	{
		SWITCH_SET(sched->socket, SWITCH_OFF);
	}

end:
	sched->run = TRUE;
}