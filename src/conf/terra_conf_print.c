#include "terra_conf.h"

#include <stdio.h>

void terra_conf_print(terra_conf const * const conf)
{
	printf("terra_conf:");
	printf("[global] tran_pin: %u\n", conf->tran_pin);
	printf("[global] tran_rep: %u\n", conf->tran_rep);
	printf("[global] sock_chan: %u\n", conf->sock_chan);
	printf("[global] sock_code_a_on: %u\n", conf->sock_code_a_on);
	printf("[global] sock_code_a_off: %u\n", conf->sock_code_a_off);
	printf("[global] sock_code_b_on: %u\n", conf->sock_code_b_on);
	printf("[global] sock_code_b_off: %u\n", conf->sock_code_b_off);
	printf("[global] sock_code_c_on: %u\n", conf->sock_code_c_on);
	printf("[global] sock_code_c_off: %u\n", conf->sock_code_c_off);
}