#include "terra.h"

#define ARG_MODE_SWITCH "switch"
#define ARG_MODE_LED "led"
#define ARG_MODE_HYGRO "hygro"
#define ARG_MODE_VISUAL "show"
#define ARG_MODE_CONFIG "conf"

int main(int argc, char ** argv)
{
	terra_led_cmd led_cmd;
	terra_switch_req switch_req;
	terra_visual_cmd visual_cmd;
	float temp, humi;

//initialization and setup
	if (!terra_runtime_init(CONF_PATH))
	{
		terra_log_error("[terra] failed to initialize runtime\n");
		exit(1);
	}

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

		terra_switch_set(&switch_req);
	}
	else if (strcmp(argv[1], ARG_MODE_LED) == 0)
	{
		if (!(led_cmd = terra_led_arg(argc, argv)))
			return 1;

		if (!terra_led_set_from_cmd(led_cmd))
			return 1;
	}
	else if (strcmp(argv[1], ARG_MODE_HYGRO) == 0)
	{
		if (!terra_hygro_run(&humi, &temp))
			return 1;

		terra_log_info("temperature %.2f, humigity: %.2f\n", temp, humi);
	}
	else if (strcmp(argv[1], ARG_MODE_VISUAL) == 0)
	{
		if (!terra_visual_arg(&visual_cmd, argc, argv))
			return 1;

		if (!terra_visual_show(visual_cmd))
			return 1;
	}
	else if (strcmp(argv[1], ARG_MODE_CONFIG) == 0)
	{
		terra_conf_print(&CONF_GLOBAL);
	}
	else
	{
		terra_log_error("invalid mode\n");
		return 1;
	}

	return 0;
}
