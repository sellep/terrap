#include "terra_conf.h"

static terra_parse_result parse_hygro_set(terra_conf_hygro_set * const set, config_setting_t * const lib)
{
	set->target = HYGRO_NONE;

	if (config_parse_float(&set->humi_act, lib, "humi_activation"))
	{
		if (config_parse_float(&set->humi_deact, lib, "humi_deactivation") == CONFIG_PARSE_OK)
		{
			set->target = HYGRO_HUMI;
		}
		else
		{
			terra_log_error("[hygro_parse_set] missing humidity deactivation\n");
			return CONFIG_PARSE_FAILED;
		}
	}

	if (config_parse_float(&set->temp_act, lib, "temp_activation"))
	{
		if (config_parse_float(&set->temp_deact, lib, "temp_deactivation") == CONFIG_PARSE_OK)
		{
			set->target |= HYGRO_TEMP;
		}
		else
		{
			terra_log_error("[hygro_parse_set] missing temperature deactivation\n");
			return CONFIG_PARSE_FAILED;
		}
	}

	return CONFIG_PARSE_OK;
}

terra_parse_result terra_conf_parse_schedule_hygro(terra_conf_schedule_hygro * * const hygros, int * const len, config_t * const lib)
{
	config_setting_t *lib_hygros;
	config_setting_t *lib_hygro;
	config_setting_t *lib_modes;
	config_setting_t *lib_mode;
	size_t i, j;

	if (!(lib_hygros = config_lookup(lib, "hygros")))
	{
		len[0] = 0;
		return CONFIG_PARSE_OK;
	}

	len[0] = config_setting_length(lib_hygros);
	if (len[0] == 0)
		return CONFIG_PARSE_OK;

	hygros[0] = (terra_conf_schedule_hygro*) malloc(sizeof(terra_conf_schedule_hygro) * len[0]);

	for (i = 0 ; i < len[0]; i++)
	{
		lib_hygro = config_setting_get_elem(lib_hygros, i);

		if (terra_conf_parse_schedule(&hygros[0][i].schedule, lib_hygro, SCHEDULE_HYGRO) != CONFIG_PARSE_OK)
		{
			terra_log_error("[terra_conf_hygro_parse] failed to parse schedule (%zu)\n", i);
			return CONFIG_PARSE_FAILED;
		}

		if (parse_hygro_set(&hygros[0][i].default_set, lib_hygro) != CONFIG_PARSE_OK)
		{
			terra_log_error("[terra_conf_hygro_parse] failed to parse default set (%s)\n", hygros[0][i].schedule.name);
			return CONFIG_PARSE_FAILED;
		}

		if (!(lib_modes = config_setting_lookup(lib_hygro, "modes")))
		{
			hygros[0][i].mode_len = 0;
			continue;
		}

		hygros[0][i].mode_len = config_setting_length(lib_modes);
		if (hygros[0][i].mode_len == 0)
			continue;

		hygros[0][i].modes = (terra_hygro_mode*) malloc(sizeof(terra_hygro_mode) * hygros[0][i].mode_len);

		for (j = 0; j < hygros[0][i].mode_len; j++)
		{
			lib_mode = config_setting_get_elem(lib_modes, j);

			if (config_parse_string(&hygros[0][i].modes[j].name, lib_mode, "mode") != CONFIG_PARSE_OK)
			{
				terra_log_error("[terra_conf_hygro_parse] failed to parse mode name (%zu)\n", j);
				return CONFIG_PARSE_FAILED;
			}

			if (parse_hygro_set(&hygros[0][i].modes[j].set, lib_mode) != CONFIG_PARSE_OK)
			{
				terra_log_error("[terra_conf_hygro_parse] failed to parse mode set (%s)\n", hygros[0][i].modes[j].name);
				return CONFIG_PARSE_FAILED;
			}
		}
	}

	return CONFIG_PARSE_OK;
}

void terra_conf_print_schedule_hygro(terra_conf_schedule_hygro const * const hygro)
{
	size_t m;

	terra_conf_print_schedule(&hygro->schedule);

	if (hygro->default_set.target & HYGRO_HUMI)
	{
		printf("humi_activation (default) = %.2f\n", hygro->default_set.humi_act);
		printf("humi_deactivation (default) = %.2f\n", hygro->default_set.humi_deact);
	}
	if (hygro->default_set.target & HYGRO_TEMP)
	{
		printf("temp_activation (default) = %.2f\n", hygro->default_set.temp_act);
		printf("temp_deactivation (default) = %.2f\n", hygro->default_set.temp_deact);
	}

	for (m = 0; m < hygro->mode_len; m++)
	{
		if (hygro->modes[m].set.target & HYGRO_HUMI)
		{
			printf("humi_activation (%s) = %.2f\n", hygro->modes[m].name, hygro->modes[m].set.humi_act);
			printf("humi_deactivation (%s) = %.2f\n", hygro->modes[m].name, hygro->modes[m].set.humi_deact);
		}

		if (hygro->modes[m].set.target & HYGRO_TEMP)
		{
			printf("temp_activation (%s) = %.2f\n", hygro->modes[m].name, hygro->modes[m].set.temp_act);
			printf("temp_deactivation (%s) = %.2f\n", hygro->modes[m].name, hygro->modes[m].set.temp_deact);
		}
	}
}

void terra_conf_free_schedule_hygro(terra_conf_schedule_hygro * const hygros, int const hygro_len)
{
	size_t h, m;

	for (h = 0; h < hygro_len; h++)
	{
		terra_conf_free_schedule(&hygros[h].schedule);

		for (m = 0; m < hygros[h].mode_len; m++)
		{
			free(hygros[h].modes[m].name);
		}

		if (hygros[h].mode_len > 0)
		{
			free(hygros[h].modes);
		}
	}

	if (hygro_len > 0)
	{
		free(hygros);
	}
}