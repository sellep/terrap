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

//initialization and setup
	if (!terra_init())
	{
		terra_log_error("failed to initialize terra\n");
		return 1;
	}
	
	return 0;

	if (!terra_conf_read(&conf, TERRA_CONF_PATH))
		return 1;

#ifdef DEBUG
	//terra_conf_print(&conf);
#endif
	return 0;

	if (!terra_conf_valid(&conf))
		return 1;

	terra_pin_set_output(conf.switch_pin);
	terra_pin_set_output(conf.led_pin_heart);

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

		terra_switch_set(&conf, &switch_req);
		return 0;
	}
	else if (strcmp(argv[1], ARG_MODE_LED) == 0)
	{
		if (!(led_cmd = terra_led_arg(argc, argv)))
			return 1;

		if (!terra_led_set_from_cmd(&conf, led_cmd))
			return 1;

		return 0;
	}
	else if (strcmp(argv[1], ARG_MODE_HYGRO) == 0)
	{
		float t;
		float h;

		if (terra_hygro_read_rep(&conf, &h, &t))
		{
			terra_log_error("failed to read hygrometer\n");
			return 1;
		}

		terra_log_info("temperatur: %f.2, humidity: %f.2\n", t, h);

		return 0;
	}
	else
	{
		terra_log_error("invalid mode\n");
		return 1;
	}

	return 0;
}
