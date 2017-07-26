#ifndef __P_TERRA_CONF_PERIOD_H
#define __P_TERRA_CONF_PERIOD_H

typedef struct
{
	terra_schedule schedule;

	size_t act;
	size_t deact;
	BOOL act_first;
	BOOL run_act_first;

	terra_time start;
} terra_schedule_period;

#endif