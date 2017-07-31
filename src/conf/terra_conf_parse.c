#include "terra_conf.h"

terra_parse_result config_parse_float(float * const f, config_setting_t * const lib, char const * const name)
{
	int i;
	double d;

	if (config_setting_lookup_float(lib, name, &d) == CONFIG_TRUE)
	{
		f[0] = (float) d;
		return CONFIG_PARSE_OK;
	}

	if (config_setting_lookup_int(lib, name, &i) == CONFIG_TRUE)
	{
		f[0] = (float) i;
		return CONFIG_PARSE_OK;
	}

	return CONFIG_PARSE_FAILED;
}

terra_parse_result config_parse_string(char * * const dest, config_setting_t * const lib, char const * const name)
{
	char *str;

	if (config_setting_lookup_string(lib, name, &str) != CONFIG_TRUE)
		return CONFIG_PARSE_UNSET;

	dest[0] = (char*) malloc(sizeof(char) * (strlen(str)));
	strcpy(dest[0], str);
	return CONFIG_PARSE_OK;
}

terra_parse_result config_parse_time(terra_time * const time, config_setting_t * const lib, char const * const name)
{
	char *str;

	if (config_setting_lookup_string(lib, name, &str) != CONFIG_TRUE)
		return CONFIG_PARSE_UNSET;

	if (terra_time_parse(&time, str, HOUR_MIN_SEC))
		return CONFIG_PARSE_OK;

	if (terra_time_parse(&time, str, HOUR_MIN))
		return CONFIG_PARSE_OK;

	return CONFIG_PARSE_FAILED;
}

static void global_parse(terra_conf * const dest, config_t * const lib)
{
	config_lookup_bool(lib, "read_only", &dest->read_only);
	config_lookup_int(lib, "delay", &dest->delay);
}

static void led_parse(terra_conf_led * const dest, config_t * const lib)
{
	config_lookup_int(lib, "led.err_pin", &dest->err_pin);
	config_lookup_int(lib, "led.heart_pin", &dest->heart_pin);
	config_lookup_int(lib, "led.heart_tick", &dest->heart_tick);
	config_lookup_int(lib, "led.heart_duration", &dest->heart_duration);
}

static void switch_groups_parse(terra_conf_switch * const dest, config_setting_t const * const lib)
{
	config_setting_t *lib_groups;
	config_setting_t *lib_group;
	size_t g;

	lib_groups = config_setting_lookup(lib, "groups");

	dest->group_len = config_setting_length(lib_groups);
	dest->groups = (terra_switch_group*) malloc(sizeof(terra_switch_group) * dest->group_len);

	for (g = 0; g < dest->group_len; g++)
	{
		lib_group = config_setting_get_elem(lib_groups, g);

		config_setting_lookup_int(lib_group, "channel", &dest->groups[g].channel);
		config_setting_lookup_int(lib_group, "code_aon", &dest->groups[g].code_aon);
		config_setting_lookup_int(lib_group, "code_aoff", &dest->groups[g].code_aoff);
		config_setting_lookup_int(lib_group, "code_bon", &dest->groups[g].code_bon);
		config_setting_lookup_int(lib_group, "code_boff", &dest->groups[g].code_boff);
		config_setting_lookup_int(lib_group, "code_con", &dest->groups[g].code_con);
		config_setting_lookup_int(lib_group, "code_coff", &dest->groups[g].code_coff);
	}
}

static void switch_parse(terra_conf * const dest, config_t * const lib)
{
	config_setting_t *lib_switch;

	lib_switch = config_lookup(lib, "switch");

	config_setting_lookup_int(lib_switch, "pin", &dest->sw.pin);
	config_setting_lookup_int(lib_switch, "repeats", &dest->sw.repeats);

	switch_groups_parse(&dest->sw, lib_switch);
}

static void hygro_parse(terra_conf * const dest, config_t * const lib)
{
	terra_time time;

	config_lookup_bool(lib, "hygro.enabled", &dest->hy.enabled);
	config_lookup_int(lib, "hygro.pin", &dest->hy.pin);
	config_lookup_int(lib, "hygro.repeats", &dest->hy.repeats);
	config_parse_time(&time, lib, "hygro.delay");

	dest->hy.delay = terra_time_to_int(&time);
}

terra_parse_result terra_conf_parse(terra_conf * const conf, char const * const path)
{
	config_t lib;
	int status = CONFIG_PARSE_FAILED;

	config_init(&lib);

	if (!config_read_file(&lib, path))
	{
		terra_log_error("[terra_conf_parse] %s(%d) - %s\n",
			config_error_file(&lib),
			config_error_line(&lib),
			config_error_text(&lib));
		goto exit;
	}

	global_parse(conf, &lib);
	led_parse(&conf->ld, &lib);
	switch_parse(conf, &lib);
	hygro_parse(conf, &lib);

	/*if (terra_conf_parse_schedule_clock(&conf->clocks, &conf->clock_len, &lib) != CONFIG_PARSE_OK)
	{
		terra_log_error("[terra_conf_parse] failed to parse clock schedules\n");
		goto exit;
	}

	if (terra_conf_parse_schedule_hygro(&conf->hygros, &conf->hygro_len, &lib) != CONFIG_PARSE_OK)
	{
		terra_log_error("[terra_conf_parse] failed to parse hygro schedules\n");
		goto exit;
	}

	if (terra_conf_parse_schedule_period(&conf->periods, &conf->period_len, &lib) != CONFIG_PARSE_OK)
	{
		terra_log_error("[terra_conf_parse] failed to parse period schedules\n");
		goto exit;
	}*/

	status = CONFIG_PARSE_OK;

exit:
	config_destroy(&lib);
	return status;
}
