#include "terra_schedule.h"

void terra_schedule_init_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);

	if (SCHEDULE_ENABLED(sched))
	{
		sched->enabled = terra_schedule_dep_enabled(sched);

		if (SCHEDULE_DISABLED(sched))
		{
			terra_log_info("[terra_schedule_init_clock] disabled schedule %s\n", sched->name);
		}
	}
}

static BOOL clock_get_time(terra_time * * const time, terra_schedule_clock const * const clock)
{
	size_t i;

	if (RUNTIME_MODE == NULL)
	{
		time[0] = &clock->time;
		return TRUE;
	}

	for (i = 0; i < clock->mode_len; i++)
	{
		if (strcmp(RUNTIME_MODE, clock->modes[i].name) == 0)
		{
			time[0] = &clock->modes[i].time;
			return TRUE;
		}
	}

	time[0] = &clock->time;
	return TRUE;
}

void terra_schedule_run_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);
	terra_start_stop *time;

	if (!terra_schedule_dep_check(sched))
		goto end;

	if(!clock_get_time(&time, clock))
		goto end;

	if (
			terra_time_diff(&NOW, &time->start) == 0
		||	terra_time_between(&NOW, &time->start, &time->stop)
	)
	{
		if (RUNTIME_SWITCH_NOT_ON(sched->socket))
		{
			SCHEDULE_SWITCH_SET_ON(sched);
		}
	}

	if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
	{
		SCHEDULE_SWITCH_SET_OFF(sched);
	}

end:
	sched->run = TRUE;
}
