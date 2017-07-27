#include "terra_schedule.h"

/*static BOOL clock_set_timep(terra_schedule_clock * const clock)
{
	size_t i;

	if (RUNTIME_MODE == NULL)
	{
		if (clock->time_def_set)
		{
			clock->time = &clock->time_def;
			return TRUE;
		}

		return FALSE;
	}

	for (i = 0; i < clock->mode_len; i++)
	{
		if (strcmp(RUNTIME_MODE, clock->modes[i].name) == 0)
		{
			clock->time = &clock->modes[i].time;
			return TRUE;
		}
	}

	return FALSE;
}*/

static inline BOOL clock_mode_init(terra_schedule_clock * const clock, terra_schedule const * const sched)
{
	size_t i;

	if (RUNTIME_DEFAULT_MODE())
	{
		if (clock->default_time_set)
		{
			clock->time = &clock->default_time;
			return TRUE;
		}
	}
	else
	{
		for (i = 0; i < clock->mode_len; i++)
		{
			if (strcmp(RUNTIME_MODE, clock->modes[i].name))
			{
				clock->time = &clock->modes[i].time;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL terra_schedule_clock_init(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);

	if (SCHEDULE_DISABLED(sched))
		return FALSE;

	//dependency init

	if (!SCHEDULE_DEP_INIT(sched))
	{
		SCHEDULE_DISABLE(sched);
		terra_log_info("[terra_schedule_clock_init] schedule %s disabled by dependency\n", sched->name);
	}

	//mode init

	else if (!clock_mode_init(clock, sched))
	{
		SCHEDULE_DISABLE(sched);
		terra_log_info("[terra_schedule_clock_init] schedule %s disabled by mode\n", sched->name);
	}

	return sched->enabled;
}

void terra_schedule_clock_run(terra_schedule_clock * const clock, terra_schedule * const sched)
{
	if (!SCHEDULE_DEP_RUN(sched))
		goto end;

	if (
			terra_time_diff(&NOW, &clock->time->start) == 0
		||	terra_time_between(&NOW, &clock->time->start, &clock->time->stop)
	)
	{
		if (RUNTIME_SWITCH_NOT_ON(sched->socket))
		{
			SCHEDULE_SWITCH_SET_ON(sched);
		}
	}
	else if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
	{
		SCHEDULE_SWITCH_SET_OFF(sched);
	}

end:
	SCHEDULE_RUN_FINISHED(sched);
}
