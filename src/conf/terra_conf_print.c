#include "terra_conf.h"

void terra_conf_print(terra_conf const * const c)
{
	ssize_t i, j;

	printf("###### global ######\n");
	printf("delay = %i\n", c->delay);
	printf("read_only = %i\n", c->read_only);
	printf("pin_alert = %i\n", c->pin_alert);

	printf("\n###### heart ######\n");
	printf("pin = %i\n", c->he.pin);
	printf("tick = %i\n", c->he.tick);
	printf("duration = %i\n", c->he.duration);

	printf("\n###### switch ######\n");
	printf("pin = %i\n", c->sw.pin);
	printf("repeats = %i\n", c->sw.repeats);
	printf("channel = %i\n", c->sw.channel);
	printf("code_aon = %i\n", c->sw.code_aon);
	printf("code_aoff = %i\n", c->sw.code_aoff);
	printf("code_bon = %i\n", c->sw.code_bon);
	printf("code_boff = %i\n", c->sw.code_boff);
	printf("code_con = %i\n", c->sw.code_con);
	printf("code_coff = %i\n", c->sw.code_coff);

	printf("\n###### hygro ######\n");
	printf("enabled = %i\n", c->hy.enabled);
	printf("pin = %i\n", c->hy.pin);
	printf("repeats = %i\n", c->hy.repeats);
	printf("delay = %i\n", c->hy.delay);

	printf("\n###### schedules ######\n");

	for (i = 0; i < c->clock_len; i++)
	{
		printf("\n###### clock %zu ######\n", i);
		printf("name = %s\n", c->clocks[i].schedule.name);
		printf("socket = %c\n", c->clocks[i].schedule.socket);
		printf("enabled = %i\n", c->clocks[i].schedule.enabled);

		for (j = 0; j < c->clocks[i].time_len; j++)
		{
			printf("time %i = ", j);
			terra_time_printnl(&c->clocks[i].times[j].start);
			printf(" - ");
			terra_time_printnl(&c->clocks[i].times[j].stop);
			printf("\n");
		}
	}
}
