#include "terra_conf.h"

static inline int clock_parse_start_stop(terra_start_stop * const time, config_setting_t * const lib)
{
	char *str;

	if (!config_setting_lookup_string(lib, "start", &str))
		return FALSE;

	if (!terra_time_parse(&time->start, str, HOUR_MIN_SEC))
	{
		terra_log_error("[clock_parse_start_stop] invalid start time (%s)\n", str);
		return -1;
	}

	if (!config_setting_lookup_string(lib, "stop", &str))
	{
		terra_log_error("[clock_parse_start_stop] missing stop time\n");
		return -1;
	}
	if (!terra_time_parse(&time->stop, str, HOUR_MIN_SEC))
	{
		terra_log_error("[clock_parse_start_stop] invalid stop time (%s)\n", str);
		return -1;
	}

	return TRUE;
}

BOOL terra_conf_schedule_clock_parse(terra_conf_schedule_clock * * const clocks, int * const len, config_t * const lib)
{
	config_setting_t *lib_clocks;
	config_setting_t *lib_clock;
	config_setting_t *lib_modes;
	config_setting_t *lib_mode;
	size_t i, j;
	int status;
	char *str;

	if (!(lib_clocks = config_lookup(lib, "clocks")))
	{
		len[0] = 0;
		return TRUE;
	}

	len[0] = config_setting_length(lib_clocks);
	if (len[0] == 0)
		return TRUE;

	clocks[0] = (terra_conf_schedule_clock*) malloc(sizeof(terra_conf_schedule_clock) * len[0]);

	for (i = 0; i < len[0]; i++)
	{
		lib_clock = config_setting_get_elem(lib_clocks, i);

		if (!terra_conf_schedule_parse(&clocks[0][i].schedule, lib_clock, SCHEDULE_CLOCK))
		{
			terra_log_error("[terra_conf_clock_parse] failed to parse schedule (%zu)\n", i);
			return FALSE;
		}

		status = clock_parse_start_stop(&clocks[0][i].default_time, lib_clock);
		if (status == -1)
		{
			terra_log_error("[terra_conf_clock_parse] failed to parse default start stop time (%s)\n", clocks[0][i].schedule.name);
			return FALSE;
		}

		clocks[0][i].default_time_set = status;

		if (!(lib_modes = config_setting_lookup(lib_clock, "modes")))
			return TRUE;

		clocks[0][i].mode_len = config_setting_length(lib_modes);
		if (clocks[0][i].mode_len == 0)
			return TRUE;

		clocks[0][i].modes = (terra_conf_clock_mode*) malloc(sizeof(terra_conf_clock_mode) * clocks[0][i].mode_len);

		for (j = 0; j < clocks[0][i].mode_len; j++)
		{
			lib_mode = config_setting_get_elem(lib_modes, j);

			if (!config_setting_lookup_string(lib_mode, "name", &str))
			{
				terra_log_error("[terra_conf_clock_parse] failed to parse mode name (%zu)\n", j);
				return FALSE;
			}

			config_string_copy(&clocks[0][i].modes[j].name, src);

			if (clock_parse_start_stop(&clocks[0][i].modes[j].time, lib_mode) != TRUE)
			{
				terra_log_error("[terra_conf_clock_parse] failed to parse mode start stop time (%s)\n", clocks[0][i].modes[j].name);
				return FALSE;
			}
		}
	}

	return TRUE;
}

void terra_conf_schedule_clock_print(terra_conf_schedule_clock const * const clock)
{
	size_t m;

	terra_conf_schedule_print(&clock->schedule);

	printf("time (default) = ");
	if (clock->default_time_set)
	{
		terra_start_stop_print(&clock->default_time);
	}
	else
	{
		printf("(null)\n");
	}

	for (m = 0; m < clock->mode_len; m++)
	{
		printf("mode (%s) = ", clock->modes[m].name);
		terra_start_stop_print(&clock->modes[m].time);
	}
}

void terra_conf_schedule_clock_free(terra_conf_schedule_clock * const clocks, int const clock_len)
{
	size_t c, m;

	for (c = 0; c < clock_len; c++)
	{
		terra_conf_schedule_free(&clocks[c].schedule);

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