#include "terra.h"
#include "conf/terra_conf.h"
#include "switch/terra_switch.h"

#define TERRA_CONF_PATH "res/terra.default.conf"

#define ARG_MODE_SWITCH "switch"

extern BOOL terra_arg_switch(terra_switch_req * const, int const, char const * const * const);

int main(int argc, char ** argv)
{
	terra_conf conf;
	terra_switch_req switch_req;

//configuration and setup
	if (!terra_lock_init())
	{
		terra_log_error("failed to initialize terra lock\n");
		return 1;
	}

	if (!terra_conf_read(&conf, TERRA_CONF_PATH))
		return 1;

	terra_conf_print(&conf);

	terra_switch_set_pin(conf.tran_pin);

//execution handling
	if (argc < 2)
	{
		terra_log_error("insufficient argument count\n");
		return 1;
	}

	if (strcmp(argv[1], ARG_MODE_SWITCH) == 0)
	{
		if (!terra_switch_arg(&switch_req, argc, argv))
			return 1;

		//terra_switch_set(&conf, &switch_req);
		return 0;
	}
	else
	{
		terra_log_error("invalid mode\n");
		return 1;
	}

	return 0;
}
