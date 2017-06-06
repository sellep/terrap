#include "terra.h"
#include "conf/terra_conf.h"
#include "switch/terra_switch.h"

#include <stdio.h>
#include <string.h>

#define TERRA_CONF_PATH "/etc/conf.d/terra.conf"

#define ARG_MODE_SWITCH "switch"

extern BOOL terra_arg_power(terra_switch_req * const, int const, char const * const * const);

int main(int argc, char ** argv)
{
	terra_conf conf;
	//terra_switch_req switch_req;

//configuration and setup

	if (!terra_conf_read_glob(&conf, TERRA_CONF_PATH))
		return 1;

	terra_conf_print(&conf);
	//terra_setup(&conf);

//execution handling

	//if (argc != 2)
	//{
	//	fprintf(stderr, "insufficient argument count\n");
	//	return 1;
	//}
    //
	//if (strcmp(argv[1], ARG_MODE_SWITCH) == 0)
	//{
	//	if (!terra_arg_switch(&switch_req, argc, argv))
	//		return 1;
    //
	//	terra_switch_set(&conf, &switch_req);
    //
	//	return 0;
	//}

	return 0;
}