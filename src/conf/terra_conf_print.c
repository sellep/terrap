#include "terra_conf.h"

static inline void print_schedule(terra_schedule const * const sched)
{
	printf("name = %s\n", sched->name);

	printf("socket = %c\n", sched->socket);
	if (sched->channel > 0)
	{
		printf("channel = %i\n", sched->channel);
	}
	else
	{
		printf("channel = -\n");
	}

	printf("enabled = %i\n", sched->enabled);
	printf("depends = %s\n", sched->dep);
}

void terra_conf_print(terra_conf const * const c)
{
	terra_time time;
	ssize_t i, j;

	printf("###### global ######\n");
	printf("delay = %i\n", c->delay);
	printf("read_only = %i\n", c->read_only);

	printf("\n###### led ######\n");
	printf("err_pin = %i\n", c->ld.err_pin);
	printf("heart_pin = %i\n", c->ld.heart_pin);
	printf("heart_tick = %i\n", c->ld.heart_tick);
	printf("heart_duration = %i\n", c->ld.heart_duration);

	printf("\n###### switch ######\n");
	printf("pin = %i\n", c->sw.pin);
	printf("repeats = %i\n", c->sw.repeats);

	for (i = 0; i < c->sw.group_len; i++)
	{
		printf("\n###### group %hu ######\n", i);

		printf("channel = %i\n", c->sw.groups[i].channel);
		printf("code_aon = %i\n", c->sw.groups[i].code_aon);
		printf("code_aoff = %i\n", c->sw.groups[i].code_aoff);
		printf("code_bon = %i\n", c->sw.groups[i].code_bon);
		printf("code_boff = %i\n", c->sw.groups[i].code_boff);
		printf("code_con = %i\n", c->sw.groups[i].code_con);
		printf("code_coff = %i\n", c->sw.groups[i].code_coff);
	}
	
	printf("\n###### hygro ######\n");
	printf("enabled = %i\n", c->hy.enabled);
	printf("pin = %i\n", c->hy.pin);
	printf("repeats = %i\n", c->hy.repeats);
	printf("delay = %i\n", c->hy.delay);

	printf("\n###### schedules ######\n");

	for (i = 0; i < c->clock_len; i++)
	{
		printf("\n###### clock %hu ######\n", i);
		print_schedule(&c->clocks[i].schedule);

		printf("times = %i\n", c->clocks[i].time_len);

		for (j = 0; j < c->clocks[i].time_len; j++)
		{
			printf("time(%hu) = ", j);
			terra_start_stop_print(&c->clocks[i].times[j]);
		}
	}

	for (i = 0; i < c->temp_len; i++)
	{
		printf("\n###### temp %hu ######\n", i);
		print_schedule(&c->temps[i].schedule);

		printf("activation = %.2f\n", c->temps[i].act);
		printf("deactivation = %.2f\n", c->temps[i].deact);
	}

	for (i = 0; i < c->period_len; i++)
	{
		printf("\n###### period %hu ######\n", i);
		print_schedule(&c->periods[i].schedule);

		terra_time_from_int(&time, c->periods[i].act);
		printf("activation = ");
		terra_time_print(&time);

		terra_time_from_int(&time, c->periods[i].deact);
		printf("deactivation = ");
		terra_time_print(&time);

		printf("active_first = %i\n", c->periods[i].act_first);
	}
}
