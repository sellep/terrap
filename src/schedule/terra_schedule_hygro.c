#include "terra_schedule.h"

static inline BOOL hygro_mode_init(terra_schedule_hygro * const hygro, terra_schedule const * const sched)
{
	size_t i;

	if (RUNTIME_DEFAULT_MODE())
	{
		if (hygro->default_set.target != HYGRO_NONE)
		{
			hygro->set = &hygro->default_set;
			return TRUE;
		}
	}
	else
	{
		for (i = 0; i < hygro->mode_len; i++)
		{
			if (strcmp(RUNTIME_MODE, hygro->modes[i].name) == 0)
			{
				hygro->set = &hygro->modes[i].set;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL terra_schedule_hygro_init(terra_schedule_hygro * const hygro)
{
	terra_schedule *sched = SCHEDULE(hygro);

	if (SCHEDULE_DISABLED(sched))
		return FALSE;

	//dependency init

	if (!SCHEDULE_DEP_INIT(sched))
	{
		SCHEDULE_DISABLE(sched);
		terra_log_info("[terra_schedule_hygro_init] schedule %s disabled by dependency\n", sched->name);
	}

	//mode init

	else if (!hygro_mode_init(hygro, sched))
	{
		SCHEDULE_DISABLE(sched);
		terra_log_info("[terra_schedule_hygro_init] schedule %s disabled by mode\n", sched->name);
	}

	return sched->enabled;
}

void terra_schedule_hygro_run(terra_schedule_hygro * const hygro, terra_schedule * const sched)
{
	if (!SCHEDULE_DEP_RUN(sched))
		goto end;

	if (hygro->set->target == HYGRO_HUMI)
	{
		if (RUNTIME_SWITCH_NOT_ON(sched->socket))
		{
			if (RUNTIME_HUMI > hygro->set->humi_act)
			{
				SCHEDULE_SWITCH_SET_ON(sched);
			}
		}
		else if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
		{
			if (RUNTIME_HUMI < hygro->set->humi_deact)
			{
				SCHEDULE_SWITCH_SET_OFF(sched);
			}
		}
	}
	else if (hygro->set->target == HYGRO_TEMP)
	{
		if (RUNTIME_SWITCH_NOT_ON(sched->socket))
		{
			if (RUNTIME_TEMP > hygro->set->temp_act)
			{
				SCHEDULE_SWITCH_SET_ON(sched);
			}
		}
		else if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
		{
			if (RUNTIME_TEMP < hygro->set->temp_deact)
			{
				SCHEDULE_SWITCH_SET_OFF(sched);
			}
		}
	}
	else
	{
		if (RUNTIME_SWITCH_NOT_ON(sched->socket))
		{
			if (RUNTIME_HUMI > hygro->set->humi_act || RUNTIME_TEMP > hygro->set->temp_act)
			{
				SCHEDULE_SWITCH_SET_ON(sched);
			}
		}
		else if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
		{
			if (RUNTIME_HUMI < hygro->set->humi_deact || RUNTIME_TEMP < hygro->set->temp_deact)
			{
				SCHEDULE_SWITCH_SET_OFF(sched);
			}
		}
	}

end:
	SCHEDULE_RUN_FINISHED(sched);
}

