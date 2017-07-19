#include "terra_schedule.h"

#define MIN_RUN 60
#define MIN_PAUSE 600

void terra_schedule_init_temp(terra_schedule_temp * const temp)
{
	temp->start.hour = 0;
	temp->start.min = 0;
	temp->start.sec = 0;
}

void terra_schedule_run_temp(terra_schedule_temp * const temp)
{
	terra_schedule *sched = SCHEDULE(temp);
	size_t start;

	if (!terra_schedule_depcheck(sched))
		goto end;

	if (RUNTIME_SWITCH_ON(sched->socket))
	{
		if (RUNTIME_TEMP >= temp->deact)
			goto end;

		start = terra_time_diff(&NOW, &temp->start);
		if (start < MIN_RUN)
			goto end;

		RUNTIME_SWITCH_SET_OFF(sched->socket);
		terra_time_cpy(&temp->start, &NOW);
	}
	else if (RUNTIME_SWITCH_OFF(sched->socket))
	{
		if (RUNTIME_TEMP < temp->act)
			goto end;

		start = terra_time_diff(&NOW, &temp->start);
		if (start < MIN_PAUSE)
			goto end;

		RUNTIME_SWITCH_SET_ON(sched->socket);
		terra_time_cpy(&temp->start, &NOW);
	}
	else
	{
		if (RUNTIME_TEMP > temp->act)
		{
			RUNTIME_SWITCH_SET_ON(sched->socket);
		}
		else
		{
			RUNTIME_SWITCH_SET_OFF(sched->socket);
		}

		terra_time_cpy(&temp->start, &NOW);
	}

end:
	sched->run = TRUE;
}
