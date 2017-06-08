#include "terra_conf.h"

#define CONF_TICK "tick="
#define CONF_SWITCH_PIN "tran_pin="
#define CONF_SWITCH_REP "tran_rep="
#define CONF_SWITCH_CHAN "sock_chan="
#define CONF_SWITCH_CODE_AON "switch_code_aon="
#define CONF_SWITCH_CODE_AOFF "switch_code_aoff="
#define CONF_SWITCH_CODE_BON "switch_code_bon="
#define CONF_SWITCH_CODE_BOFF "switch_code_boff="
#define CONF_SWITCH_CODE_CON "switch_code_con="
#define CONF_SWITCH_CODE_COFF "switch_code_coff="
#define CONF_HYGRO_ENABLED "hygro_enabled="
#define CONF_HYGRO_PIN "hygro_pin="
#define CONF_HYGRO_TICK "hygro_tick="
#define CONF_HYGRO_SENSOR "hygro_sensor="

BOOL terra_conf_read_global(terra_conf * const conf, FILE * const f)
{
	char *line = NULL;
	size_t buf_len = 0;
	size_t read;

	while ((read = getline(&line, &buf_len, f)) != -1)
	{
		if (line[0] == '\0')
			continue;
		
		if (line[0] == '#')
			continue;

		if (strncmp(line, CONF_TICK, sizeof(CONF_TICK) - 1) == 0)
		{
			conf->tick = atoi(line + sizeof(CONF_TICK) - 1);
			if (conf->tick == 0)
			{
				terra_log_error("invalid tick value\n");
				return FALSE;
			}
		}
		else if (strncmp(line, CONF_SWITCH_PIN, sizeof(CONF_SWITCH_PIN) - 1) == 0)
		{
			conf->tran_pin = atoi(line + sizeof(CONF_SWITCH_PIN) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_REP, sizeof(CONF_SWITCH_REP) - 1) == 0)
		{
			conf->tran_rep = atoi(line + sizeof(CONF_SWITCH_REP) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CHAN, sizeof(CONF_SWITCH_CHAN) - 1) == 0)
		{
			conf->sock_chan = atoi(line + sizeof(CONF_SWITCH_CHAN) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_AON, sizeof(CONF_SWITCH_CODE_AON) - 1) == 0)
		{
			conf->sock_code_a_on = atoi(line + sizeof(CONF_SWITCH_CODE_AON) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_AOFF, sizeof(CONF_SWITCH_CODE_AOFF) - 1) == 0)
		{
			conf->sock_code_a_off = atoi(line + sizeof(CONF_SWITCH_CODE_AOFF) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_BON, sizeof(CONF_SWITCH_CODE_BON) - 1) == 0)
		{
			conf->sock_code_b_on = atoi(line + sizeof(CONF_SWITCH_CODE_BON) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_BOFF, sizeof(CONF_SWITCH_CODE_BOFF) - 1) == 0)
		{
			conf->sock_code_b_off = atoi(line + sizeof(CONF_SWITCH_CODE_BOFF) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_CON, sizeof(CONF_SWITCH_CODE_CON) - 1) == 0)
		{
			conf->sock_code_c_on = atoi(line + sizeof(CONF_SWITCH_CODE_CON) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_CODE_COFF, sizeof(CONF_SWITCH_CODE_COFF) - 1) == 0)
		{
			conf->sock_code_c_off = atoi(line + sizeof(CONF_SWITCH_CODE_COFF) - 1);
		}
		if (strncmp(line, CONF_HYGRO_ENABLED, sizeof(CONF_HYGRO_ENABLED) - 1) == 0)
		{
			conf->hygro_enabled = atoi(line + sizeof(CONF_HYGRO_ENABLED) - 1);
		}
		if (strncmp(line, CONF_HYGRO_PIN, sizeof(CONF_HYGRO_PIN) - 1) == 0)
		{
			conf->hygro_pin = atoi(line + sizeof(CONF_HYGRO_PIN) - 1);
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
	}

	if (line)
	{
		free(line);
	}

	return TRUE;
}
