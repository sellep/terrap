#include "terra.h"

/*static inline void change_switch(terra_sched_temp const * const temp, ssize_t const i, switch_mode const mode)
{
	terra_switch_req req;

	_switch_modes[i] = mode;

	req.sock = temp->sched.sock;
	req.set_on = mode == SWITCH_ON ? TRUE : FALSE;
	terra_switch_set(&req);
}*/

void terrad_run_temp_init()
{
	/*terra_sched_temp *temp;
	ssize_t i;

	for (i = 0 ; i < conf.sched_temps_len; i++)
	{
		_switch_modes[i] = SWITCH_OFF;
	}*/
}

void terrad_run_temp(float const t)
{
	/*
	terra_sched_temp *temp;
	ssize_t i;

	for (i = 0 ; i < conf.sched_temps_len; i++)
	{
		temp = &conf.sched_temps[i];

		if (SCHED_DISABLED(temp))
			continue;

		if (IS_SWITCH_OFF(i))
		{
			if (t > temp->act_lim)
			{
				change_switch(temp, i, SWITCH_ON);
			}
		}
		else if (IS_SWITCH_ON(i))
		{
			if (t < temp->deact_lim)
			{
				change_switch(temp, i, SWITCH_OFF);
			}
		}
	}*/
}
