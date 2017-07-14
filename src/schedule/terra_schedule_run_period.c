#include "terra_schedule.h"

void terra_schedule_run_period(terra_schedule_period * const period)
{
	terra_schedule *sched = SCHEDULE(period);

	sched->run = TRUE;
}
