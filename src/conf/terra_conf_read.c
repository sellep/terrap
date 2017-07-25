#include "terra_conf.h"

#define SCHEDULE_SET_TYPE(s, t) (s)->type = (t)

static inline string_copy(char * * const dest, char const * const src)
{
	dest[0] = (char*) malloc(sizeof(char) * (strlen(src)));
	strcpy(dest[0], src);
}

static inline int parse_start_stop(terra_start_stop * const time, config_setting_t * const src)
{
	char *str;

	if (!config_setting_lookup_string(src, "start", &str))
		return FALSE;

	if (!terra_time_parse(&time->start, str, HOUR_MIN_SEC))
	{
		terra_log_error("[parse_start_stop] invalid start time (%s)\n", str);
		return -1;
	}

	if (!config_setting_lookup_string(src, "stop", &str))
	{
		terra_log_error("[parse_start_stop] missing stop time\n");
		return -1;
	}
	if (!terra_time_parse(&time->stop, str, HOUR_MIN_SEC))
	{
		terra_log_error("[parse_start_stop] invalid stop time (%s)\n", str);
		return -1;
	}

	return TRUE;
}

static inline BOOL parse_float(float * const f, config_setting_t const * const src, char const * const name)
{
	int i;
	double d;

	if (config_setting_lookup_float(src, name, &d) == CONFIG_TRUE)
	{
		f[0] = (float) d;
		return TRUE;
	}

	if (config_setting_lookup_int(src, name, &i) == CONFIG_TRUE)
	{
		f[0] = (float) i;
		return TRUE;
	}

	return FALSE;
}

static inline void terra_conf_global_parse(terra_conf * const dest, config_t const * const src)
{
	config_lookup_bool(src, "read_only", &dest->read_only);
	config_lookup_int(src, "delay", &dest->delay);
}

static inline void terra_conf_led_parse(terra_conf_led * const dest, config_t const * const src)
{
	config_lookup_int(src, "led.err_pin", &dest->err_pin);
	config_lookup_int(src, "led.heart_pin", &dest->heart_pin);
	config_lookup_int(src, "led.heart_tick", &dest->heart_tick);
	config_lookup_int(src, "led.heart_duration", &dest->heart_duration);
}

static inline void terra_conf_switch_groups_parse(terra_conf_switch * const dest, config_setting_t const * const src)
{
	config_setting_t *src_groups;
	config_setting_t *src_group;
	size_t g;

	src_groups = config_setting_lookup(src, "groups");

	dest->group_len = config_setting_length(src_groups);
	dest->groups = (terra_switch_group*) malloc(sizeof(terra_switch_group) * dest->group_len);

	for (g = 0; g < dest->group_len; g++)
	{
		src_group = config_setting_get_elem(src_groups, g);

		config_setting_lookup_int(src_group, "channel", &dest->groups[g].channel);
		config_setting_lookup_int(src_group, "code_aon", &dest->groups[g].code_aon);
		config_setting_lookup_int(src_group, "code_aoff", &dest->groups[g].code_aoff);
		config_setting_lookup_int(src_group, "code_bon", &dest->groups[g].code_bon);
		config_setting_lookup_int(src_group, "code_boff", &dest->groups[g].code_boff);
		config_setting_lookup_int(src_group, "code_con", &dest->groups[g].code_con);
		config_setting_lookup_int(src_group, "code_coff", &dest->groups[g].code_coff);
	}
}

static inline void terra_conf_switch_parse(terra_conf * const dest, config_t const * const src)
{
	config_setting_t *src_switch;

	src_switch = config_lookup(src, "switch");

	config_setting_lookup_int(src_switch, "pin", &dest->sw.pin);
	config_setting_lookup_int(src_switch, "repeats", &dest->sw.repeats);

	terra_conf_switch_groups_parse(&dest->sw, src_switch);
}

static inline void terra_conf_hygro_parse(terra_conf * const dest, config_t const * const src)
{
	terra_time time;
	char *str;

	config_lookup_bool(src, "hygro.enabled", &dest->hy.enabled);
	config_lookup_int(src, "hygro.pin", &dest->hy.pin);
	config_lookup_int(src, "hygro.repeats", &dest->hy.repeats);
	config_lookup_string(src, "hygro.delay", &str);

	terra_time_parse(&time, str, HOUR_MIN_SEC);
	dest->hy.delay = terra_time_to_int(&time);
}

static BOOL terra_conf_schedule_parse(terra_schedule * const sched, config_setting_t const * const src)
{
	char *str;

	config_setting_lookup_string(src, "name", &str);
	string_copy(&sched->name, str);

	config_setting_lookup_string(src, "socket", &str);
	sched->socket = str[0];

	if (strlen(str) > 2)
	{
		if (str[1] != ':')
		{
			terra_log_error("[terra_conf_schedule_parse] socket syntax error\n");
			return FALSE;
		}

		sched->channel = atoi(str + 2);
	}
	else
	{
		sched->channel = 0;
	}

	config_setting_lookup_bool(src, "enabled", &sched->enabled);

	if (config_setting_lookup_string(src, "depends", &str) == CONFIG_TRUE)
	{
		string_copy(&sched->dep, str);
	}

	return TRUE;
}

static BOOL terra_conf_clock_mode_parse(terra_schedule_clock * const dest, config_setting_t * const src)
{
	config_setting_t *src_modes;
	config_setting_t *src_mode;
	char *str;
	size_t i;

	src_modes = config_setting_lookup(src, "modes");
	if (src_modes == NULL)
	{
		dest->mode_len = 0;
		return TRUE;
	}

	dest->mode_len = config_setting_length(src_modes);
	if (dest->mode_len == 0)
		return TRUE;

	dest->modes = (terra_schedule_clock_mode*) malloc(sizeof(terra_schedule_clock_mode) * dest->mode_len);

	for (i = 0; i < dest->mode_len; i++)
	{
		src_mode = config_setting_get_elem(src_modes, i);

		if (!config_setting_lookup_string(src_mode, "mode", &str))
		{
			terra_log_error("[terra_conf_clock_mode_parse] missing mode name\n");
			return FALSE;
		}

		string_copy(&dest->modes[i].name, str);

		if (parse_start_stop(&dest->modes[i].time, src_mode) <= 0)
		{
			terra_log_error("[terra_conf_clock_mode_parse] error parsing start stop time\n");
			return FALSE;
		}
	}

	return TRUE;
}

static BOOL terra_conf_clocks_parse(terra_conf * const dest, config_t const * const src)
{
	config_setting_t *src_clocks;
	config_setting_t *src_clock;
	char *str;
	size_t i;
	int result;

	src_clocks = config_lookup(src, "clocks");

	dest->clock_len = config_setting_length(src_clocks);
	dest->clocks = (terra_schedule_clock*) malloc(sizeof(terra_schedule_clock) * dest->clock_len);

	for (i = 0; i < dest->clock_len; i++)
	{
		src_clock = config_setting_get_elem(src_clocks, i);

		if (!terra_conf_schedule_parse(&dest->clocks[i].schedule, src_clock))
		{
			terra_log_error("[terra_conf_clocks_parse] failed to parse schedule\n");
			return FALSE;
		}

		SCHEDULE_SET_TYPE(&dest->clocks[i].schedule, SCHEDULE_CLOCK);

		result = parse_start_stop(&dest->clocks[i].time_val, src_clock);
		if (result == -1)
		{
			terra_log_error("[terra_conf_clocks_parse] failed to parse start stop time\n");
			return FALSE;
		}

		&dest->clocks[i].time_def = result;

		if (!terra_conf_clock_mode_parse(&dest->clocks[i], src_clock))
		{
			terra_log_error("[terra_conf_clocks_parse] failed to parse clock modes\n");
			return FALSE;
		}
	}

	return TRUE;
}

static BOOL terra_conf_temps_parse(terra_conf * const dest, config_t const * const src)
{
	config_setting_t *src_temps;
	config_setting_t *src_temp;
	char *str;
	size_t i;
	double val;

	src_temps = config_lookup(src, "temps");

	dest->temp_len = config_setting_length(src_temps);
	dest->temps = (terra_schedule_temp*) malloc(sizeof(terra_schedule_temp) * dest->temp_len);

	for (i = 0; i < dest->temp_len; i++)
	{
		src_temp = config_setting_get_elem(src_temps, i);

		if (!terra_conf_schedule_parse(&dest->temps[i].schedule, src_temp))
		{
			terra_log_error("[terra_conf_temps_parse] failed to parse schedule\n");
			return FALSE;
		}

		SCHEDULE_SET_TYPE(&dest->temps[i].schedule, SCHEDULE_TEMP);

		if (!parse_float(&dest->temps[i].act, src_temp, "activation"))
		{
			terra_log_error("[terra_conf_temps_parse] unable to parse activation for temp schedule %zu\n", i);
			return FALSE;
		}
		if (!parse_float(&dest->temps[i].deact, src_temp, "deactivation"))
		{
			terra_log_error("[terra_conf_temps_parse] unable to parse deactivation for temp schedule %zu\n", i);
			return FALSE;
		}
	}

	return TRUE;
}

static BOOL terra_conf_periods_parse(terra_conf * const dest, config_t const * const src)
{
	config_setting_t *src_periods;
	config_setting_t *src_period;
	terra_time time;
	char *str;
	size_t i;

	src_periods = config_lookup(src, "periods");

	dest->period_len = config_setting_length(src_periods);
	dest->periods = (terra_schedule_period*) malloc(sizeof(terra_schedule_period) * dest->period_len);

	for (i = 0; i < dest->period_len; i++)
	{
		src_period = config_setting_get_elem(src_periods, i);

		if (!terra_conf_schedule_parse(&dest->periods[i].schedule, src_period))
		{
			terra_log_error("[terra_conf_periods_parse] failed to parse schedule\n");
			return FALSE;
		}


		SCHEDULE_SET_TYPE(&dest->periods[i].schedule, SCHEDULE_PERIOD);

		if (!config_setting_lookup_string(src_period, "active", &str))
		{
			terra_log_error("[terra_conf_periods_parse] unable to read active (%zu)\n", i);
			return FALSE;
		}
		if (!terra_time_parse(&time, str, HOUR_MIN_SEC))
		{
			terra_log_error("[terra_conf_periods_parse] unable to parse active (%s)\n", str);
			return FALSE;
		}
		dest->periods[i].act = terra_time_to_int(&time);

		if (!config_setting_lookup_string(src_period, "deactive", &str))
		{
			terra_log_error("[terra_conf_periods_parse] unable to read deactive (%zu)\n", i);
			return FALSE;
		}
		if (!terra_time_parse(&time, str, HOUR_MIN_SEC))
		{
			terra_log_error("[terra_conf_periods_parse] unable to parse deactive (%s)\n", str);
			return FALSE;
		}
		dest->periods[i].deact = terra_time_to_int(&time);

		config_setting_lookup_bool(src_period, "active_first", &dest->periods[i].act_first);
	}

	return TRUE;
}

BOOL terra_conf_read(terra_conf * const dest, char const * const path)
{
	BOOL status = FALSE;
	config_t libconf;

	config_init(&libconf);

	if (!config_read_file(&libconf, path))
	{
		terra_log_error("[terra_conf_read] %s:%d - %s\n",
			config_error_file(&libconf),
			config_error_line(&libconf),
			config_error_text(&libconf));
		goto exit;
	}

	terra_conf_global_parse(dest, &libconf);
	terra_conf_led_parse(&dest->ld, &libconf);
	terra_conf_switch_parse(dest, &libconf);
	terra_conf_hygro_parse(dest, &libconf);

	if (!terra_conf_clocks_parse(dest, &libconf))
	{
		terra_log_error("[terra_conf_read] failed to parse clocks\n");
		goto exit;
	}

	if (!terra_conf_temps_parse(dest, &libconf))
	{
		terra_log_error("[terra_conf_read] failed to parse temps\n");
		goto exit;
	}

	if (!terra_conf_periods_parse(dest, &libconf))
	{
		terra_log_error("[terra_conf_read] failed to parse periods\n");
		goto exit;
	}

	status = TRUE;

exit:
	config_destroy(&libconf);
	return status;
}
