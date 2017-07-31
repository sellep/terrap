#include "terra_conf.h"

static int parse_clock_start_stop(terra_start_stop * const time, config_setting_t * const lib)
{
	terra_parse_result status;

	status = config_parse_time(&time->start, lib, "start");

	if (status == CONFIG_PARSE_FAILED)
	{
		terra_log_error("[parse_clock_start_stop] invalid start time\n");
		return CONFIG_PARSE_FAILED;
	}

	if (status == CONFIG_PARSE_UNSET)
		return CONFIG_PARSE_UNSET;

	status = config_parse_time(&time->stop, lib, "stop");

	if (status == CONFIG_PARSE_FAILED)
	{
		terra_log_error("[parse_clock_start_stop] invalid stop time\n");
		return CONFIG_PARSE_FAILED;
	}

	if (status == CONFIG_PARSE_UNSET)
	{
		terra_log_error("[parse_clock_start_stop] missing stop time\n");
		return CONFIG_PARSE_FAILED;
	}

	return CONFIG_PARSE_OK;
}

terra_parse_result terra_conf_parse_schedule_clock(terra_conf_schedule_clock * * const clocks, int * const len, config_t * const lib)
{
	config_setting_t *lib_clocks;
	config_setting_t *lib_clock;
	config_setting_t *lib_modes;
	config_setting_t *lib_mode;
	size_t i, j;
	terra_parse_result status;

	lib_clocks = config_lookup(lib, "clocks");

	len[0] = config_setting_length(lib_clocks);
	if (len[0] == 0)
		return CONFIG_PARSE_OK;

	clocks[0] = (terra_conf_schedule_clock*) malloc(sizeof(terra_conf_schedule_clock) * len[0]);

	for (i = 0; i < len[0]; i++)
	{
		lib_clock = config_setting_get_elem(lib_clocks, i);

		if (terra_conf_parse_schedule(&clocks[0][i].schedule, lib_clock, SCHEDULE_CLOCK) != CONFIG_PARSE_OK)
		{
			terra_log_error("[terra_conf_parse_schedule_clock] failed to parse schedule (%zu)\n", i);
			return FALSE;
		}

		status = parse_clock_start_stop(&clocks[0][i].default_time, lib_clock);
		if (status == CONFIG_PARSE_FAILED)
		{
			terra_log_error("[terra_conf_parse_schedule_clock] failed to parse default start stop time (%s)\n", clocks[0][i].schedule.name);
			return CONFIG_PARSE_FAILED;
		}

		clocks[0][i].default_time_set = status == CONFIG_PARSE_OK ? TRUE : FALSE;

		lib_modes = config_setting_lookup(lib_clock, "modes");
			return CONFIG_PARSE_OK;

		clocks[0][i].mode_len = config_setting_length(lib_modes);
		if (clocks[0][i].mode_len == 0)
			return CONFIG_PARSE_OK;

		clocks[0][i].modes = (terra_conf_clock_mode*) malloc(sizeof(terra_conf_clock_mode) * clocks[0][i].mode_len);

		for (j = 0; j < clocks[0][i].mode_len; j++)
		{
			lib_mode = config_setting_get_elem(lib_modes, j);

			if (config_parse_string(&clocks[0][i].modes[j].name, lib_mode, "mode") != CONFIG_PARSE_OK)
			{
				terra_log_error("[terra_conf_parse_schedule_clock] failed to parse mode name (%zu)\n", j);
				return CONFIG_PARSE_FAILED;
			}

			if (parse_clock_start_stop(&clocks[0][i].modes[j].time, lib_mode) != CONFIG_PARSE_OK)
			{
				terra_log_error("[terra_conf_parse_schedule_clock] failed to parse mode start stop time (%s)\n", clocks[0][i].modes[j].name);
				return CONFIG_PARSE_FAILED;
			}
		}
	}

	return CONFIG_PARSE_OK;
}

void terra_conf_print_schedule_clock(terra_conf_schedule_clock const * const clock)
{
	size_t m;

	terra_conf_print_schedule(&clock->schedule);

	printf("time (default) = ");
	if (clock->default_time_set)
	{
		terra_start_stop_print(&clock->default_time);
	}
	else
	{
		printf("(null)\n");
	}
	
	printf("clock mode len : %i\n", clock->mode_len);

	for (m = 0; m < clock->mode_len; m++)
	{
		printf("mode (%s) = ", clock->modes[m].name);
		terra_start_stop_print(&clock->modes[m].time);
	}
}

void terra_conf_free_schedule_clock(terra_conf_schedule_clock * const clocks, int const clock_len)
{
	size_t c, m;

	for (c = 0; c < clock_len; c++)
	{
		terra_conf_free_schedule(&clocks[c].schedule);

		for (m = 0; m < clocks[c].mode_len; m++)
		{
			free(clocks[c].modes[m].name);
		}

		if (clocks[c].mode_len > 0)
		{
			free(clocks[c].modes);
		}
	}

	if (clock_len > 0)
	{
		free(clocks);
	}
}