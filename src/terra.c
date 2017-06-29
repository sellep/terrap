#include "terra.h"

#define TERRA_CONF_PATH "/etc/default/terra"

#define ARG_MODE_SWITCH "switch"
#define ARG_MODE_LED "led"
#define ARG_MODE_HYGRO "hygro"

extern BOOL terra_init();

int main(int argc, char ** argv)
{
	terra_conf conf;
	terra_led_cmd led_cmd;
	terra_switch_req switch_req;
	terra_hygro_cmd hygro_cmd;

//initialization and setup
	if (!terra_init())
	{
		terra_log_error("[terra] failed to initialize terra\n");
		return 1;
	}

	if (!terra_conf_read(&conf, TERRA_CONF_PATH))
		return 1;

#ifdef DEBUG
	terra_conf_print(&conf);
#endif

	if (!terra_conf_valid(&conf))
		return 1;

	terra_pin_set_output(conf.switch_pin);
	terra_pin_set_output(conf.led_pin_heart);

//execution handling
	if (argc < 2)
	{
		terra_log_error("[terra] insufficient argument count\n");
		return 1;
	}

	if (strcmp(argv[1], ARG_MODE_SWITCH) == 0)
	{
		if (!terra_switch_arg(&switch_req, argc, argv))
			return 1;

		terra_switch_set(&conf, &switch_req);
	}
	else if (strcmp(argv[1], ARG_MODE_LED) == 0)
	{
		if (!(led_cmd = terra_led_arg(argc, argv)))
			return 1;

		if (!terra_led_set_from_cmd(&conf, led_cmd))
			return 1;
	}
	else if (strcmp(argv[1], ARG_MODE_HYGRO) == 0)
	{
		if (!terra_hygro_arg(&hygro_cmd, argc, argv))
			return 1;

		if (!terra_hygro_run(&conf, &hygro_cmd))
			return 1;
	}
	else
	{
		terra_log_error("invalid mode\n");
		return 1;
	}

	return 0;
}
