#include "terra_schedule.h"

void terra_schedule_run_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);
	ssize_t t;

	if (!terra_schedule_depcheck(sched))
		goto end;

	for (t = 0; t < clock->time_len; t++)
	{
		if (
				terra_time_diff(&NOW, &clock->times[t].start) == 0
			||	terra_time_between(&NOW, &clock->times[t].start, &clock->times[t].stop)
		)
		{
			if (RUNTIME_SWITCH_NOT_ON(sched->socket))
			{
				RUNTIME_SWITCH_SET_ON(sched->socket);
			}

			goto end;
		}
	}

	if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
	{
		RUNTIME_SWITCH_SET_OFF(sched->socket);
	}

end:
	sched->run = TRUE;
}
