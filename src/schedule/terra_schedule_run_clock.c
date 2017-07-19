#include "terra_schedule.h"

void terra_schedule_init_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);

	if (SCHEDULE_ENABLED(sched))
	{
		sched->enabled = terra_schedule_dep_enabled(clock);

		if (SCHEDULE_DISABLED(sched))
		{
			terra_log_info("[terra_schedule_init_clock] disabled schedule %s\n", sched->name);
		}
	}
}

void terra_schedule_run_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);
	ssize_t t;

	if (!terra_schedule_dep_check(sched))
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
				RUNTIME_SWITCH_SET_ON(sched->socket, sched->name);
			}

			goto end;
		}
	}

	if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
	{
		RUNTIME_SWITCH_SET_OFF(sched->socket, sched->name);
	}

end:
	sched->run = TRUE;
}
