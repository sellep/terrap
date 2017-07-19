#include "terra_schedule.h"

void terra_schedule_init_temp(terra_schedule_temp * const temp)
{
	terra_schedule *sched = SCHEDULE(temp);

	if (SCHEDULE_ENABLED(sched))
	{
		sched->enabled = terra_schedule_dep_enabled(temp);

		if (SCHEDULE_DISABLED(sched))
		{
			terra_log_info("[terra_schedule_init_temp] disabled schedule %s\n", sched->name);
		}
	}
}

void terra_schedule_run_temp(terra_schedule_temp * const temp)
{
	terra_schedule *sched = SCHEDULE(temp);
	size_t start;

	if (!terra_schedule_dep_check(sched))
		goto end;

	if (RUNTIME_SWITCH_ON(sched->socket))
	{
		if (RUNTIME_TEMP >= temp->deact)
			goto end;


		RUNTIME_SWITCH_SET_OFF(sched->socket, sched->name);
	}
	else if (RUNTIME_SWITCH_OFF(sched->socket))
	{
		if (RUNTIME_TEMP < temp->act)
			goto end;

		RUNTIME_SWITCH_SET_ON(sched->socket, sched->name);
	}
	else
	{
		if (RUNTIME_TEMP > temp->act)
		{
			RUNTIME_SWITCH_SET_ON(sched->socket, sched->name);
		}
		else
		{
			RUNTIME_SWITCH_SET_OFF(sched->socket, sched->name);
		}
	}

end:
	sched->run = TRUE;
}
