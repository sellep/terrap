#include "terra_schedule.h"

static inline void period_init(terra_schedule_period * const period)
{
	period->run_act_first = period->act_first;
}

void terra_schedule_init_period(terra_schedule_period * const period)
{
	terra_schedule *sched = SCHEDULE(period);

	if (SCHEDULE_ENABLED(sched))
	{
		sched->enabled = terra_schedule_dep_enabled(sched);

		if (SCHEDULE_DISABLED(sched))
		{
			terra_log_info("[terra_schedule_init_period] schedule %s disabled by dependency\n", sched->name);
		}
		else
		{
			period_init(period);
		}
	}
}

static inline void period_switch_off(terra_schedule_period * const period, terra_schedule const * const sched)
{
	SCHEDULE_SWITCH_SET_OFF(sched);
	terra_time_cpy(&period->start, &NOW);
}

static inline void period_switch_on(terra_schedule_period * const period, terra_schedule const * const sched)
{
	SCHEDULE_SWITCH_SET_ON(sched);
	terra_time_cpy(&period->start, &NOW);
}

void terra_schedule_run_period(terra_schedule_period * const period)
{
	terra_schedule *sched = SCHEDULE(period);
	size_t diff;

	if (!terra_schedule_dep_check(sched))
		goto end;

	if (period->run_act_first)
	{
		period_switch_on(period, sched);
		period->run_act_first = FALSE;
		goto end;
	}

	diff = terra_time_diff(&NOW, &period->start);

	if (RUNTIME_SWITCH_ON(sched->socket))
	{
		if (diff > period->act)
		{
			period_switch_off(period, sched);
		}
	}
	else if (RUNTIME_SWITCH_OFF(sched->socket))
	{
		if (diff > period->deact)
		{
			period_switch_on(period, sched);
		}
	}
	else
	{
		period_switch_off(period, sched);
	}

end:
	sched->run = TRUE;
}
