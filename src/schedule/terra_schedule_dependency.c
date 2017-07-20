#include "terra_schedule.h"

#define DEP_NAME(s) (SCHEDULE_INVERS_DEP(s)) ? (s)->dep + 1 : (s)->dep

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

	terra_log_error("[schedule_get_dep] dependency %s not found\n", dep_name);
	terra_exit(1);
	return NULL;
}

BOOL terra_schedule_dep_enabled(terra_schedule const * const sched)
{
	if (SCHEDULE_NO_DEP(sched))
		return sched->enabled;

	return terra_schedule_dep_enabled(terra_schedule_get_dep(sched));
}

BOOL terra_schedule_dep_check(terra_schedule const * const sched)
{
	terra_schedule *dep;

	if (SCHEDULE_NO_DEP(sched))
		return TRUE;

	dep = terra_schedule_get_dep(sched);

	if (LIKELY(SCHEDULE_IS_NOT_TEMP(dep)) && SCHEDULE_NOT_RUN(dep))
	{
		if (LIKELY(SCHEDULE_IS_CLOCK(dep)))
		{
			terra_schedule_run_clock((terra_schedule_clock*) dep);
		}
		else if (SCHEDULE_IS_PERIOD(dep))
		{
			terra_schedule_run_period((terra_schedule_period*) dep);
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