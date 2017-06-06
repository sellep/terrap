#include "terra_conf.h"

void terra_conf_print(terra_conf const * const conf)
{
	size_t i;

	printf("terra_conf:\n");
	printf("[global] tran_pin: %u\n", conf->tran_pin);
	printf("[global] tran_rep: %u\n", conf->tran_rep);
	printf("[global] sock_chan: %u\n", conf->sock_chan);
	printf("[global] sock_code_a_on: %u\n", conf->sock_code_a_on);
	printf("[global] sock_code_a_off: %u\n", conf->sock_code_a_off);
	printf("[global] sock_code_b_on: %u\n", conf->sock_code_b_on);
	printf("[global] sock_code_b_off: %u\n", conf->sock_code_b_off);
	printf("[global] sock_code_c_on: %u\n", conf->sock_code_c_on);
	printf("[global] sock_code_c_off: %u\n", conf->sock_code_c_off);

	for (i = 0; i < conf->sched_clocks_len; i++)
	{
		printf("[sched_clock %u] name: %s\n", i, conf->sched_clocks[i].sched.name);
		printf("[sched_clock %u] trigger: %s\n", i,
			conf->sched_clocks[i].sched.trig == TRIGGER_CLOCK ? "clock"
			: conf->sched_clocks[i].sched.trig == TRIGGER_TEMP ? "temp"
			: "mois");
	}
}
