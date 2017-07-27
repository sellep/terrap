#include "terra_conf.h"

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
		printf("\n###### group %zd ######\n", i);
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
		printf("\n###### clock %zd ######\n", i);
		terra_conf_schedule_clock_print(&c->clocks[i]);
	}

	for (i = 0; i < c->hygro_len; i++)
	{
		printf("\n###### hygro %zd ######\n", i);
		terra_conf_schedule_hygro_print(&c->hygros[i]);
	}
}

void terra_conf_free(terra_conf * const conf)
{
	free(conf->sw.groups);

	terra_conf_schedule_clock_free(conf->clocks, conf->clock_len);
	terra_conf_schedule_hygro_free(conf->hygros, conf->hygro_len);
}