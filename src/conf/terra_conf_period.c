#include "terra_conf.h"

static terra_parse_result parse_period_set(terra_conf_period_set * const set, config_setting_t * const lib)
{
	terra_time time;
	terra_parse_result status;

	status = config_parse_time(&time, lib, "on_time");

	if (status == CONFIG_PARSE_UNSET)
		return CONFIG_PARSE_UNSET;

	if (status == CONFIG_PARSE_FAILED)
	{
		terra_log_error("[period_parse_set] invalid on_time\n");
		return CONFIG_PARSE_FAILED;
	}

	set->on_time = terra_time_to_int(&time);

	if (status == CONFIG_PARSE_FAILED)
	{
		terra_log_error("[period_parse_set] invalid off_time\n");
		return CONFIG_PARSE_FAILED;
	}

	if (status == CONFIG_PARSE_UNSET)
	{
		terra_log_error("[period_parse_set] missing off_time\n");
		return CONFIG_PARSE_FAILED;
	}

	set->off_time = terra_time_to_int(&time);
	return CONFIG_PARSE_OK;
} 

terra_parse_result terra_conf_parse_schedule_period(terra_conf_schedule_period * * const periods,  int * const len, config_t * const lib)
{
	config_setting_t *lib_periods;
	config_setting_t *lib_period;
	config_setting_t *lib_modes;
	config_setting_t *lib_mode;
	size_t i, j;
	terra_parse_result status;

	if (!(lib_periods = config_lookup(lib, "periods")))
	{
		len[0] = 0;
		return CONFIG_PARSE_OK;
	}

	len[0] = config_setting_length(lib_periods);
	if (len[0] == 0)
		return CONFIG_PARSE_OK;

	periods[0] = (terra_conf_schedule_period*) malloc(sizeof(terra_conf_schedule_period) * len[0]);

	for (i = 0; i < len[0]; i++)
	{
		lib_period = config_setting_get_elem(lib_periods, i);

		if (terra_conf_parse_schedule(&periods[0][i].schedule, lib_period, SCHEDULE_PERIOD) != CONFIG_PARSE_OK)
		{
			terra_log_error("[terra_conf_parse_schedule_period] failed to parse schedule (%zu)\n", i);
			return CONFIG_PARSE_FAILED;
		}
		
		printf("conf period %s\n", periods[0][i].schedule.name);

		status = parse_period_set(&periods[0][i].default_set, lib_period);
		if (status == CONFIG_PARSE_FAILED)
		{
			terra_log_error("[terra_conf_parse_schedule_period] failed to parse default on/off_time\n");
			return CONFIG_PARSE_FAILED;
		}

		periods[0][i].has_default_set = status == CONFIG_PARSE_OK ? TRUE : FALSE;

		if (!(lib_modes = config_setting_lookup(lib_period, "modes")))
		{
			periods[0][i].mode_len = 0;
			continue;
		}

		periods[0][i].mode_len = config_setting_length(lib_modes);
		if (periods[0][i].mode_len == 0)
			continue;

		periods[0][i].modes = (terra_period_mode*) malloc(sizeof(terra_period_mode) * periods[0][i].mode_len);

		for (j = 0; j < periods[0][i].mode_len; j++)
		{
			lib_mode = config_setting_get_elem(lib_modes, j);

			if (config_parse_string(&periods[0][i].modes[j].name, lib_mode, "mode") != CONFIG_PARSE_OK)
			{
				terra_log_error("[terra_conf_parse_schedule_period] failed to parse mode name (%zu)\n", j);
				return CONFIG_PARSE_FAILED;
			}

			if (parse_period_set(&periods[0][i].modes[j].set, lib_mode) != CONFIG_PARSE_OK)
			{
				terra_log_error("[terra_conf_parse_schedule_period] failed to parse period set (%s)\n", periods[0][i].modes[j].name);
				return CONFIG_PARSE_FAILED;
			}
		}
	}

	return CONFIG_PARSE_OK;
}

void terra_conf_print_schedule_period(terra_conf_schedule_period const * const period)
{
	terra_time time;
	size_t m;

	terra_conf_print_schedule(&period->schedule);

	printf("period time (default) = ");
	if (period->has_default_set)
	{
		terra_time_from_int(&time, period->default_set.on_time);
		terra_time_printnl(&time);
		printf("/");
		terra_time_from_int(&time, period->default_set.off_time);
		terra_time_print(&time);
	}
	else
	{
		printf("(null)\n");
	}

	for (m = 0; m < period->mode_len; m++)
	{
		printf("period time (%s) = ", period->modes[m].name);
		terra_time_from_int(&time, period->modes[m].set.on_time);
		terra_time_printnl(&time);
		printf("/");
		terra_time_from_int(&time, period->modes[m].set.off_time);
		terra_time_print(&time);
	}
}

void terra_conf_free_schedule_period(terra_conf_schedule_period * const periods, int const period_len)
{
	size_t p, m;

	for (p = 0; p < period_len; p++)
	{
		terra_conf_free_schedule(&periods[p].schedule);

		for (m = 0; m < periods[p].mode_len; m++)
		{
			free(periods[p].modes[m].name);
		}

		if (periods[p].mode_len > 0)
		{
			free(periods[p].modes);
		}
	}

	if (period_len > 0)
	{
		free(periods);
	}
}
