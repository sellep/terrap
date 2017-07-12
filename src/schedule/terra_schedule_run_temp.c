#include "terra_schedule.h"

void terra_schedule_temp_init()
{

}

void terra_schedule_run_temp(terra_schedule_temp * const temp)
{
	terra_schedule *sched = SCHEDULE(temp);

	sched->run = TRUE;
}
