#include "terra_conf.h"

static inline string_copy(char * * const dest, char const * const src)
{
	dest[0] = (char*) malloc(sizeof(char) * (strlen(src)));
	strcpy(dest[0], src);
}

static inline void terra_conf_global_parse(terra_conf * const dest, config_t const * const src)
{
	config_lookup_bool(src, "read_only", &dest->read_only);
	config_lookup_int(src, "delay", &dest->delay);
	config_lookup_int(src, "pin_alert", &dest->pin_alert);
}

static inline void terra_conf_heart_parse(terra_conf * const dest, config_t const * const src)
{
	config_lookup_int(src, "heart.pin", &dest->he.pin);
	config_lookup_int(src, "heart.tick", &dest->he.tick);
	config_lookup_int(src, "heart.duration", &dest->he.duration);
}

static inline void terra_conf_switch_parse(terra_conf * const dest, config_t const * const src)
{ 
	config_lookup_int(src, "switch.pin", &dest->sw.pin);
	config_lookup_int(src, "switch.repeats", &dest->sw.repeats);
	config_lookup_int(src, "switch.channel", &dest->sw.channel);
	config_lookup_int(src, "switch.code_aon", &dest->sw.code_aon);
	config_lookup_int(src, "switch.code_aoff", &dest->sw.code_aoff);
	config_lookup_int(src, "switch.code_bon", &dest->sw.code_bon);
	config_lookup_int(src, "switch.code_boff", &dest->sw.code_boff);
	config_lookup_int(src, "switch.code_con", &dest->sw.code_con);
	config_lookup_int(src, "switch.code_coff", &dest->sw.code_coff);
}

static inline void terra_conf_hygro_parse(terra_conf * const dest, config_t const * const src)
{
	const terra_time time;
	const char *str;

	config_lookup_bool(src, "hygro.enabled", &dest->hy.enabled);
	config_lookup_int(src, "hygro.pin", &dest->hy.pin);
	config_lookup_int(src, "hygro.repeats", &dest->hy.repeats);
	config_lookup_string(src, "hygro.delay", &str);

	terra_time_read(&time, str);
	dest->hy.delay = terra_time_to_int(&time);
}

static void terra_conf_scheduler_parse(terra_scheduler * const scheduler, config_setting_t const * const src)
{
	char *str;
	config_setting_lookup_string(src, "name", &str);
	string_copy(&scheduler->name, str);

	config_setting_lookup_string(src, "socket", &str);
	scheduler->socket = str[0];

	config_setting_lookup_bool(src, "enabled", &scheduler->enabled);
}

static void terra_conf_clocks_parse(terra_conf * const dest, config_t const * const src)
{
	config_setting_t *src_clocks;
	config_setting_t *src_clock;
	char *str;
	size_t i, t;

	src_clocks = config_lookup(src, "clocks");

	dest->clock_len = config_setting_length(src_clocks);
	dest->clocks = (terra_scheduler_clock*) malloc(sizeof(terra_scheduler_clock) * dest->clock_len);

	for (i = 0; i < dest->clock_len; i++)
	{
		src_clock = config_setting_get_elem(src_clocks, i);

		terra_conf_scheduler_parse(&dest->clocks[i].scheduler, src_clock);

		//TODO: multi times parsing

		dest->clocks[i].time_len = 1;
		dest->clocks[i].times = (terra_start_stop*) malloc(sizeof(terra_start_stop) * dest->clocks[i].time_len);

		config_setting_lookup_string(src_clock, "start", &str);
		terra_time_read(&dest->clocks[i].times[0].start, str);

		config_setting_lookup_string(src_clock, "stop", &str);
		terra_time_read(&dest->clocks[i].times[0].stop, str);
	}
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
	terra_conf_heart_parse(dest, &libconf);
	terra_conf_switch_parse(dest, &libconf);
	terra_conf_hygro_parse(dest, &libconf);
	terra_conf_clocks_parse(dest, &libconf);

	status = TRUE;

exit:
	config_destroy(&libconf);
	return status;
}
