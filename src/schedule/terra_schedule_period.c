#include "terra_schedule.h"

static BOOL period_mode_init(terra_schedule_period * const period, terra_schedule const * const sched)
{
	size_t i;

	if (RUNTIME_DEFAULT_MODE())
	{
		if (hygro->has_defualt_set)
		{
			period->set = &period->default_set;
			return TRUE;
		}
	}
	else
	{
		for (i = 0; i < period->mode_len; i++)
		{
			if (strcmp(RUNTIME_MODE, period->modes[i].name) == 0)
			{
				period->set = &period->modes[i].set;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL terra_schedule_init_period(terra_schedule_period * const period)
{
	terra_schedule *sched = SCHEDULE(period);

	if (SCHEDULE_DISABLED(sched))
		return FALSE;

	//dependency init

	if (!SCHEDULE_DEP_INIT(sched))
	{
		SCHEDULE_DISABLE(sched);
		terra_log_info("[terra_schedule_period_init] schedule %s disabled by dependency\n", sched->name);
		return FALSE;
	}

	//mode init

	if (!period_mode_init(period, sched))
	{
		SCHEDULE_DISABLE(sched);
		terra_log_info("[terra_schedule_period_init] schedule %s disabled by mode\n", sched->name);
		return FALSE;
	}

	period->state = SWITCH_UNKNOWN;
	return TRUE;
}

static inline void period_switch_off(terra_schedule_period * const period, terra_schedule * const sched)
{
	SCHEDULE_SWITCH_SET_OFF(sched);
	terra_time_cpy(&period->start, &NOW);
	period->state = SWITCH_OFF;
}

static inline void period_switch_on(terra_schedule_period * const period, terra_schedule * const sched)
{
	SCHEDULE_SWITCH_SET_ON(sched);
	terra_time_cpy(&period->start, &NOW);
	period->state = SWITCH_ON;
}

void terra_schedule_run_period(terra_schedule_period * const period, terra_schedule * const sched)
{
	size_t diff;

	if (!SCHEDULE_DEP_RUN(sched))
		goto end;

	//handle first invoke
	if (period->state == SWITCH_UNKNOWN)
	{
		if (period->set->off_start)
		{
			if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
			{
				SCHEDULE_SWITCH_SET_OFF(sched);
			}

			period->state = SWITCH_OFF;
		}
		else
		{
			if (RUNTIME_SWITCH_NOT_ON(sched->socket))
			{
				SCHEDULE_SWITCH_SET_ON(sched);
			}

			period->state = SWITCH_ON;
		}

		terra_time_cpy(&period->start, &NOW);
		goto end;
	}

	diff = terra_time_diff(&NOW, &period->start);

	if (period->state == SWITCH_ON)
	{
		if (diff >= period->set->on_time)
		{
			period_switch_off(period, sched);
		}
	}
	else
	{
		if (diff >= period->set->off_time)
		{
			period_switch_on(period, sched);
		}
	}

end:
	SCHEDULE_RUN_FINISHED(sched);
}