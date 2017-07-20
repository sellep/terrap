#include "terra_runtime.h"
#include "visual/terra_visual.h"
#include "utils/terra_signal.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define ARG_MODE_SWITCH "switch"
#define ARG_MODE_LED "led"
#define ARG_MODE_HYGRO "hygro"
#define ARG_MODE_VISUAL "show"
#define ARG_MODE_CONFIG "conf"
#define ARG_MODE_DAEMON "daemon"
#define ARG_MODE_SCHEDULE "schedule"
#define ARG_MODE_RELOAD "reload"

int main(int argc, char ** argv)
{
	terra_led_cmd led_cmd;
	terra_switch_req switch_req;
	terra_visual_cmd visual_cmd;
	float temp, humi;

	if (argc < 2)
	{
		terra_log_error("[terra] insufficient argument count\n");
		return 1;
	}

	if (!terra_runtime_init(CONF_PATH))
	{
		terra_log_error("[terra] failed to initialize runtime\n");
		exit(1);
	}

	if (strcmp(argv[1], ARG_MODE_SWITCH) == 0)
	{
		if (!terra_switch_arg(&switch_req, argc, argv))
			return 1;

		terra_switch_set(&CONF_SWITCH, switch_req.socket, switch_req.mode, (char*) NULL);
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
		if (!terra_hygro_read(&humi, &temp, &CONF_HYGRO))
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
		terra_conf_print(&CONF_GLOBAL);
	}
	else if (strcmp(argv[1], ARG_MODE_DAEMON) == 0)
	{
		if (fork() == 0)
		{
			if (execl("/usr/local/bin/terra", "/usr/local/bin/terra", "schedule", "logfile", (char*) NULL) == -1)
			{
				terra_log_error("[terra] failed to execl (%s)\n", strerror(errno));
				exit(1);
			}
		}
	} 
	else if (strcmp(argv[1], ARG_MODE_SCHEDULE) == 0)
	{
		terra_schedule_arg(argc, argv);
		terra_schedule_run();
	}
	else if (strcmp(argv[1], ARG_MODE_RELOAD) == 0)
	{
		terra_signal_sighup();
	}
	else
	{
		terra_log_error("invalid mode\n");
		return 1;
	}

	return 0;
}
