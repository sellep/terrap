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
		sched->enabled = terra_schedule_dep_enabled(period);

		if (SCHEDULE_DISABLED(sched))
		{
			terra_log_info("[terra_schedule_init_period] disabled schedule %s\n", sched->name);
		}
		else
		{
			period_init(period);
		}
	}
}

static inline void period_switch_off(terra_schedule_period * const period, terra_schedule const * const sched)
{
	RUNTIME_SWITCH_SET_OFF(sched->socket, sched->name);
	terra_time_cpy(&period->start, &NOW);
}

static inline void period_switch_on(terra_schedule_period * const period, terra_schedule const * const sched)
{
	RUNTIME_SWITCH_SET_ON(sched->socket, sched->name);
	terra_time_cpy(&period->start, &NOW);
}

void terra_schedule_run_period(terra_schedule_period * const period)
{
	terra_schedule *sched = SCHEDULE(period);
	size_t diff;

	printf("terra_schedule_run_period\n");

	if (!terra_schedule_dep_check(sched))
		goto end;

	if (period->run_act_first)
	{
		printf("into run act first\n");
		period_switch_on(period, sched);
		period->run_act_first = FALSE;
		goto end;
	}

	diff = terra_time_diff(&NOW, &period->start);
	printf("diff %zu\n", diff);

	if (RUNTIME_SWITCH_ON(sched->socket))
	{
		printf("RUNTIME_SWITCH_ON\n");
		if (diff > period->act)
		{
			printf("period_switch_off\n");
			period_switch_off(period, sched);
		}
	}
	else if (RUNTIME_SWITCH_OFF(sched->socket))
	{
		printf("RUNTIME_SWITCH_OFF\n");
		if (diff > period->deact)
		{
			printf("period_switch_on\n");
			period_switch_on(period, sched);
		}
	}
	else
	{
		printf("period_switch_off\n");
		period_switch_off(period, sched);
	}

end:
	printf("end\n");
	sched->run = TRUE;
}
