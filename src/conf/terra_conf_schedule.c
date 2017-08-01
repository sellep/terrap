#include "terra_conf.h"

void terra_conf_print_schedule(terra_conf_schedule const * const sched)
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

terra_parse_result terra_conf_parse_schedule(terra_conf_schedule * const sched, config_setting_t * const lib, terra_schedule_type const type)
{
	char *str;

	if (config_parse_string(&sched->name, lib, "name") != CONFIG_PARSE_OK)
	{
		terra_log_error("[terra_conf_parse_schedule] failed to parse name\n");
		return CONFIG_PARSE_FAILED;
	}

	if (!config_setting_lookup_string(lib, "socket", &str))
	{
		terra_log_error("[terra_conf_parse_schedule] missing socket\n");
		return CONFIG_PARSE_FAILED;
	}

	if (strlen(str) == 0)
	{
		terra_log_error("[terra_conf_parse_schedule] missing socket value\n");
		return CONFIG_PARSE_FAILED;
	}

	sched->socket = str[0];

	if (strlen(str) > 2)
	{
		if (str[1] != ':')
		{
			terra_log_error("[terra_conf_parse_schedule] socket syntax error\n");
			return CONFIG_PARSE_FAILED;
		}

		sched->channel = atoi(str + 2);
	}
	else
	{
		sched->channel = 0;
	}

	if (!config_setting_lookup_bool(lib, "enabled", &sched->enabled))
	{
		sched->enabled = FALSE;
	}

	if (config_parse_string(&sched->dep, lib, "depends") != CONFIG_PARSE_OK)
	{
		sched->dep = NULL;
	}

	sched->type = type;

	return CONFIG_PARSE_OK;
}

void terra_conf_free_schedule(terra_conf_schedule * const sched)
{
	free(sched->name);

	if (sched->dep)
	{
		free(sched->dep);
	}
}