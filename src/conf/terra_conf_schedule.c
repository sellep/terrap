#include "terra_conf.h"

void terra_conf_schedule_print(terra_conf_schedule const * const sched)
{
	printf("name = %s\n", sched->name);

	printf("socket = %c\n", sched->socket);
	if (sched->channel > 0)
	{
		printf("channel = %i\n", sched->channel);
	}
	else
	{
		printf("channel = -\n");
	}

	printf("enabled = %i\n", sched->enabled);
	printf("depends = %s\n", sched->dep);
}

BOOL terra_conf_schedule_parse(terra_conf_schedule * const sched, config_setting_t * const lib, terra_schedule_type const type)
{
	char *str;

	if (!config_setting_lookup_string(lib, "name", &str))
	{
		terra_log_error("[terra_schedule_parse] missing name\n");
		return FALSE;
	}

	config_string_copy(&sched->name, str);

	if (!config_setting_lookup_string(lib, "socket", &str))
	{
		terra_log_error("[terra_schedule_parse] missing socket\n");
		return FALSE;
	}

	if (strlen(str) == 0)
	{
		terra_log_error("[terra_schedule_parse] missing socket value\n");
		return FALSE;
	}

	sched->socket = str[0];

	if (strlen(str) > 2)
	{
		if (str[1] != ':')
		{
			terra_log_error("[terra_schedule_parse] socket syntax error\n");
			return FALSE;
		}

		sched->channel = atoi(str + 2);
	}
	else
	{
		sched->channel = 0;
	}

	config_setting_lookup_bool(lib, "enabled", &sched->enabled);

	if (config_setting_lookup_string(lib, "depends", &str) == CONFIG_TRUE)
	{
		config_string_copy(&sched->dep, str);
	}
	else
	{
		sched->dep = NULL;
	}

	sched->type = type;

	return TRUE;
}

void terra_conf_schedule_free(terra_conf_schedule * const sched)
{
	free(sched->name);

	if (sched->dep)
	{
		free(sched->dep);
	}
}