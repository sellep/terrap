#include "terra_switch.h"

size_t terra_switch_get_sock_code(terra_conf const * const conf, terra_switch_req const * const req)
{
	if (req->sock == 'a')
	{
		if (req->set_on)
			return conf->sock_code_a_on;

		return conf->sock_code_a_off;
	}

	if (req->sock == 'b')
	{
		if (req->set_on)
			return conf->sock_code_b_on;

		return conf->sock_code_b_off;
	}

	if (req->set_on)
		return conf->sock_code_c_on;

	return conf->sock_code_c_off;
}