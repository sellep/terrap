#include "terra_runtime.h"
#include "schedule/terra_schedule.h"
#include "visual/terra_visual.h"
#include "utils/terra_signal.h"
#include "utils/terra_mode.h"

#define ARG_MODE_SWITCH "switch"
#define ARG_MODE_LED "led"
#define ARG_MODE_HYGRO "hygro"
#define ARG_MODE_VISUAL "show"
#define ARG_MODE_CONFIG "conf"
#define ARG_MODE_DAEMON "daemon"
#define ARG_MODE_MODE "mode"
#define ARG_MODE_SCHEDULE "schedule"

int main(int argc, char ** argv)
{
	terra_led_cmd led_cmd;
	terra_switch_req switch_req;
	terra_visual_cmd visual_cmd;
	float temp, humi;

	if (argc < 2)
	{
		printf("a\n");
		terra_log_error("[terra] insufficient argument count\n");
		return 1;
	}

	printf("a.0\n");
	
	if (!terra_runtime_init(CONF_PATH))
	{
		printf("a.1\n");
		terra_log_error("[terra] failed to initialize runtime\n");
		return 1;
	}

	printf("a.2\n");
	
	if (strcmp(argv[1], ARG_MODE_DAEMON) == 0)
	{
		if (!terra_daemon_run(argc, argv))
			return 1;

		goto exit;
	}
	else if (strcmp(argv[1], ARG_MODE_MODE) == 0)
	{
		if (!terra_mode_run(argc, argv))
			return 1;

		goto exit;
	}
	else if (strcmp(argv[1], ARG_MODE_SWITCH) == 0)
	{
		if (!terra_switch_run(argc, argv, runtime.switch_modes, &CONF_SWITCH))
			return 1;
	}
	else if (strcmp(argv[1], ARG_MODE_LED) == 0)
	{
		if (!(led_cmd = terra_led_arg(argc, argv)))
			return 1;

		if (!terra_led_set_from_cmd(&CONF_GLOBAL, led_cmd))
			return 1;
	}
	else if (strcmp(argv[1], ARG_MODE_HYGRO) == 0)
	{
		if (!terra_hygro_read(&humi, &temp, &CONF_HYGRO, (size_t*) NULL))
			return 1;

		terra_log_info("humidity %.1f, temperature: %.1f\n", humi, temp);
	}
	else if (strcmp(argv[1], ARG_MODE_VISUAL) == 0)
	{
		if (!terra_visual_arg(&visual_cmd, argc, argv))
			return 1;

		if (!terra_visual_show(&visual_cmd))
			return 1;
	}
	else if (strcmp(argv[1], ARG_MODE_CONFIG) == 0)
	{
		if (!terra_conf_run(argc, argv, &CONF_GLOBAL))
			return 1;
	}
	else if (strcmp(argv[1], ARG_MODE_SCHEDULE) == 0)
	{
		printf("a.5\n");
		
		if (terra_schedule_arg(argc, argv))
		{
			printf("a.6\n");
			
			terra_schedule_run();
		}
	}
	else
	{
		terra_log_error("invalid mode\n");
		return 1;
	}

exit:
	return 0;
}
