#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra.h"

typedef struct
{
	ssize_t tran_pin;
	ssize_t tran_rep;
	size_t sock_chan;
	size_t sock_code_a_on;
	size_t sock_code_a_off;
	size_t sock_code_b_on;
	size_t sock_code_b_off;
	size_t sock_code_c_on;
	size_t sock_code_c_off;
} terra_conf;

void terra_conf_print(terra_conf const * const);
BOOL terra_conf_read_glob(terra_conf * const, char const * const);
BOOL terra_setup(terra_conf const * const);

#endif