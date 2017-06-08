#include "terra_conf.h"

#define CONF_TICK "tick="
#define CONF_LED_PIN_WARN "led_pin_warn="
#define CONF_SWITCH_PIN "switch_pin="
#define CONF_SWITCH_REP "switch_rep="
#define CONF_SWITCH_CHAN "switch_chan="
#define CONF_SWITCH_CODE_AON "switch_code_aon="
#define CONF_SWITCH_CODE_AOFF "switch_code_aoff="
#define CONF_SWITCH_CODE_BON "switch_code_bon="
#define CONF_SWITCH_CODE_BOFF "switch_code_boff="
#define CONF_SWITCH_CODE_CON "switch_code_con="
#define CONF_SWITCH_CODE_COFF "switch_code_coff="
#define CONF_HYGRO_ENABLED "hygro_enabled="
#define CONF_HYGRO_PIN_IO "hygro_pin_io="
#define CONF_HYGRO_TICK "hygro_tick="
#define CONF_HYGRO_SENSOR "hygro_sensor="
#define CONF_GLOBAL_END "GLOBAL END"

BOOL terra_conf_read_global(terra_conf * const conf, FILE * const f)
{
	char *line = NULL;
	size_t buf_len = 0;
	size_t read;

	while ((read = getline(&line, &buf_len, f)) != -1)
	{
		if (line[0] == '\0')
			continue;

		if (line[0] == '\n')
			continue;
		
		if (line[0] == '#')
			continue;

		if (strncmp(line, CONF_TICK, sizeof(CONF_TICK) - 1) == 0)
		{
			conf->tick = atoi(line + sizeof(CONF_TICK) - 1);
			if (conf->tick == 0)
			{
				terra_log_error("tick cannot be zero\n");
				return FALSE;
			}
		}
		else if (strncmp(line, CONF_LED_PIN_WARN, sizeof(CONF_LED_PIN_WARN) - 1) == 0)
		{
			conf->led_pin_warn = atoi(line + sizeof(CONF_LED_PIN_WARN) - 1);
			if (conf->led_pin_warn == 0)
			{
				terra_log_error("led_pin_warn cannot use pin zero\n");
				return FALSE;
			}
		}
		else if (strncmp(line, CONF_SWITCH_PIN, sizeof(CONF_SWITCH_PIN) - 1) == 0)
		{
			conf->switch_pin = atoi(line + sizeof(CONF_SWITCH_PIN) - 1);
			if (conf->switch_pin == 0)
			{
				terra_log_error("switch_pin cannot use pin zero\n");
				return FALSE;
			}
		}
		else if (strncmp(line, CONF_SWITCH_REP, sizeof(CONF_SWITCH_REP) - 1) == 0)
		{
			conf->switch_rep = atoi(line + sizeof(CONF_SWITCH_REP) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CHAN, sizeof(CONF_SWITCH_CHAN) - 1) == 0)
		{
			conf->switch_chan = atoi(line + sizeof(CONF_SWITCH_CHAN) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_AON, sizeof(CONF_SWITCH_CODE_AON) - 1) == 0)
		{
			conf->switch_code_aon = atoi(line + sizeof(CONF_SWITCH_CODE_AON) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_AOFF, sizeof(CONF_SWITCH_CODE_AOFF) - 1) == 0)
		{
			conf->switch_code_aoff = atoi(line + sizeof(CONF_SWITCH_CODE_AOFF) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_BON, sizeof(CONF_SWITCH_CODE_BON) - 1) == 0)
		{
			conf->switch_code_bon = atoi(line + sizeof(CONF_SWITCH_CODE_BON) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_BOFF, sizeof(CONF_SWITCH_CODE_BOFF) - 1) == 0)
		{
			conf->switch_code_boff = atoi(line + sizeof(CONF_SWITCH_CODE_BOFF) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_CON, sizeof(CONF_SWITCH_CODE_CON) - 1) == 0)
		{
			conf->switch_code_con = atoi(line + sizeof(CONF_SWITCH_CODE_CON) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_COFF, sizeof(CONF_SWITCH_CODE_COFF) - 1) == 0)
		{
			conf->switch_code_coff = atoi(line + sizeof(CONF_SWITCH_CODE_COFF) - 1);
		}
		else if (strncmp(line, CONF_HYGRO_ENABLED, sizeof(CONF_HYGRO_ENABLED) - 1) == 0)
		{
			conf->hygro_enabled = atoi(line + sizeof(CONF_HYGRO_ENABLED) - 1);
		}
		else if (strncmp(line, CONF_HYGRO_PIN_IO, sizeof(CONF_HYGRO_PIN_IO) - 1) == 0)
		{
			conf->hygro_pin_io = atoi(line + sizeof(CONF_HYGRO_PIN_IO) - 1);
			if (conf->hygro_pin_io == 0)
			{
				terra_log_error("hygro_pin_io cannot use pin zero\n");
				return FALSE;
			}
		}
		else if (strncmp(line, CONF_HYGRO_TICK, sizeof(CONF_HYGRO_TICK) - 1) == 0)
		{
			conf->hygro_tick = atoi(line + sizeof(CONF_HYGRO_TICK) - 1);
			if (conf->hygro_tick == 0)
			{
				terra_log_error("invalid hygro_tick value\n");
				return FALSE;
			}
		}
		else if (strncmp(line, CONF_HYGRO_SENSOR, sizeof(CONF_HYGRO_SENSOR) - 1) == 0)
		{
			strncpy(conf->hygro_sensor, line + sizeof(CONF_HYGRO_SENSOR) - 1, read - sizeof(CONF_HYGRO_SENSOR));
			conf->hygro_sensor[read - sizeof(CONF_HYGRO_SENSOR)] = '\0';
		}
		else if (strncmp(line, CONF_GLOBAL_END, sizeof(CONF_GLOBAL_END) - 1) == 0)
		{
			break;
		}
		else
		{
			terra_log_error("unknown config line: %s\n", line);
			return FALSE;
		}
	}

	if (line)
	{
		free(line);
	}

	return TRUE;
}
