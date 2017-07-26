#include "terra_schedule_hygro.h"

#include "../conf/terra_conf_hygro.h"

static inline BOOL hygro_mode_check(terra_schedule_hygro * const hygro, terra_schedule const * const sched)
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
			if (strcmp(RUNTIME_MODE, hygro->modes[i].name))
			{
				hygro->set = &hygro->modes[i].set;
				return TRUE;
			}
		}
	}

	return FALSE;
}

void terra_schedule_hygro_init(terra_schedule_hygro * const hygro)
{
	terra_schedule *sched = SCHEDULE(hygro);

	if (SCHHEDULE_DISABLED(sched))
		return;

	//dependency check

	SCHEDULE_DEP_INIT(sched);

	if (SCHEDULE_DEP_DISABLED(sched))
	{
		SCHEDULE_DISABLE(sched);
		terra_log_info("[terra_schedule_hygro_init] schedule %s disabled by dependency\n", sched->name);
		return;
	}

	//mode check

	if (!hygro_mode_check(hygro, sched))
	{
		SCHEDULE_DISABLE(sched);
		terra_log_info("[terra_schedule_hygro_init] schedule %s disabled by mode\n", sched->name);
	}
}

void terra_schedule_hygro_run(terra_conf_hygro const * const hygro)
{
	terra_schedule *sched = SCHEDULE(hygro);

	if (!SCHEDULE_DEP_RUN(sched))
		return;

	if (hygro->set->target == HYGRO_HUMI)
	{
		if (RUNTIME_SWITCH_NOT_ON(sched->socket))
		{
			if (RUNTIME_HUMI > hygro->set->humi_act)
			{
				RUNTIME_SWITCH_SET_ON(sched);
			}
		}
		else if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
		{
			if (RUNTIME_HUMI < hugro->set->humi_deact)
			{
				RUNTIME_SWITCH_SET_OFF(sched);
			}
		}
	}
	else if (hygro->set->target == HYGRO_TEMP)
	{
		if (RUNTIME_SWITCH_NOT_ON(sched->socket))
		{
			if (RUNTIME_TEMP > hygro->set->temp_act)
			{
				RUNTIME_SWITCH_SET_ON(sched);
			}
		}
		else if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
		{
			if (RUNTIME_TEMP < hugro->set->temp_deact)
			{
				RUNTIME_SWITCH_SET_OFF(sched);
			}
		}
	}
	else
	{
		if (RUNTIME_SWITCH_NOT_ON(sched->socket))
		{
			if (RUNTIME_HUMI > hygro->set->humi_act || RUNTIME_TEMP > hygro->set->temp_act)
			{
				RUNTIME_SWITCH_SET_ON(sched);
			}
		}
		else if (RUNTIME_SWITCH_NOT_OFF(sched->socket))
		{
			if (RUNTIME_HUMI < hygro->set->humi_deact || RUNTIME_TEMP < hygro->set->temp_deact)
			{
				RUNTIME_SWITCH_SET_ON(sched);
			}
		}
	}
}

