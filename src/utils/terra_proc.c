#include "terra_proc.h"

#include <sched.h>

/*
void set_max_priority()
{
	struct sched_param sched;
	memset(&sched, 0, sizeof(sched));

	sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
	sched_setscheduler(0, SCHED_FIFO, &sched);
}

void set_default_priority()
{
	struct sched_param sched;
	memset(&sched, 0, sizeof(sched));

	sched.sched_priority = 0;
	sched_setscheduler(0, SCHED_OTHER, &sched);
}*/
