#include "terra_schedule.h"

static BOOL clock_set_timep(terra_schedule_clock * const clock)
{
	size_t i;

	if (RUNTIME_MODE == null)
	{
		if (clock->time_def)
		{
			clock->timep = &clock->time;
			return TRUE;
		}

		return FALSE;
	}

	for (i = 0; i < clock->mode_len; i++)
	{
		if (strcmp(RUNTIME_MODE, clock->modes[i].name) == 0)
		{
			clock->timep = &clock->modes[i].time;
			return TRUE;
		}
	}

	return FALSE;
}

void terra_schedule_init_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);

	if (SCHEDULE_ENABLED(sched))
	{
		sched->enabled = terra_schedule_dep_enabled(sched);
		if (SCHEDULE_DISABLED(sched))
		{
			terra_log_info("[terra_schedule_init_clock] schedule %s disabled by dependency\n", sched->name);
			return;
		}

		sched->enabled = clock_set_timep(clock);
		if (SCHEDULE_DISABLED(sched))
		{
			terra_log_info("[terra_schedule_init_clock] schedule %s disabled by mode\n", sched->name);
		}
	}
}

void terra_schedule_run_clock(terra_schedule_clock * const clock)
{
	terra_schedule *sched = SCHEDULE(clock);

	if (!terra_schedule_dep_check(sched))
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
