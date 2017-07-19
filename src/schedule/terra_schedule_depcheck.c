#include "terra_schedule.h"

#define DEP_NAME(s) SCHEDULE_INVERS_DEP(s) ? (s)->dep + 1 : (s)->dep

static inline terra_schedule* terra_schedule_get_dep(terra_schedule const * const sched)
{
	terra_schedule *target;
	char *dep_name = DEP_NAME(sched);
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		target = SCHEDULE(SCHEDULE_GET_CLOCK(i));

		if (strcmp(dep_name, target->name) == 0)
			return target;
	}

	for (i = 0; i < CONF_GLOBAL.temp_len; i++)
	{
		target = SCHEDULE(SCHEDULE_GET_TEMP(i));

		if (strcmp(dep_name, target->name) == 0)
			return target;
	}

	//terra_exit(1);
	terra_log_error("[schedule_get_dep] dependency %s not found\n", dep_name);
	return NULL;
}

BOOL terra_schedule_depcheck(terra_schedule const * const sched)
{
	terra_schedule *dep;

	if (SCHEDULE_NO_DEP(sched))
		return TRUE;

	dep = terra_schedule_get_dep(sched);
	printf("dep of %s is %s\n", sched->name, dep->name);

	if (UNLIKELY(SCHEDULE_NOT_RUN(dep)))
	{
		if (SCHEDULE_IS_CLOCK(dep))
		{
			terra_schedule_run_clock(dep);
		}
		else if (SCHEDULE_IS_PERIOD(dep))
		{
			terra_schedule_run_period(dep);
		}
		else
		{
			terra_schedule_run_temp(dep);
		}
	}

	if (SCHEDULE_INVERS_DEP(sched))
	{
		if (RUNTIME_SWITCH_OFF(dep->socket))
			return TRUE;
	}
	else if (RUNTIME_SWITCH_ON(dep->socket))
		return TRUE;

	return FALSE;
}