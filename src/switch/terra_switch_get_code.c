#include "terra_switch.h"

size_t terra_switch_get_code(terra_conf const * const conf, terra_switch_req const * const req)
{
	if (req->sock == 'a')
	{
		if (req->set_on)
			return conf->switch_code_aon;

		return conf->switch_code_aoff;
	}

	if (req->sock == 'b')
	{
		if (req->set_on)
			return conf->switch_code_bon;

		return conf->switch_code_boff;
	}

	if (req->set_on)
		return conf->switch_code_con;

	return conf->switch_code_coff;
}