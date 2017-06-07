#include "terra_conf.h"

#define CONF_TICK "tick="
#define CONF_TRAN_PIN "tran_pin="
#define CONF_TRAN_REP "tran_rep="
#define CONF_SOCK_CHAN "sock_chan="
#define CONF_SOCK_CODE_A_ON "sock_code_a_on="
#define CONF_SOCK_CODE_A_OFF "sock_code_a_off="
#define CONF_SOCK_CODE_B_ON "sock_code_b_on="
#define CONF_SOCK_CODE_B_OFF "sock_code_b_off="
#define CONF_SOCK_CODE_C_ON "sock_code_c_on="
#define CONF_SOCK_CODE_C_OFF "sock_code_c_off="
#define CONF_HYGRO_ENABLED "hygro_enabled="
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
		else if (strncmp(line, CONF_TRAN_PIN, sizeof(CONF_TRAN_PIN) - 1) == 0)
		{
			conf->tran_pin = atoi(line + sizeof(CONF_TRAN_PIN) - 1);
		}
		else if (strncmp(line, CONF_TRAN_REP, sizeof(CONF_TRAN_REP) - 1) == 0)
		{
			conf->tran_rep = atoi(line + sizeof(CONF_TRAN_REP) - 1);
		}
		else if (strncmp(line, CONF_SOCK_CHAN, sizeof(CONF_SOCK_CHAN) - 1) == 0)
		{
			conf->sock_chan = atoi(line + sizeof(CONF_SOCK_CHAN) - 1);
		}
		else if (strncmp(line, CONF_SOCK_CODE_A_ON, sizeof(CONF_SOCK_CODE_A_ON) - 1) == 0)
		{
			conf->sock_code_a_on = atoi(line + sizeof(CONF_SOCK_CODE_A_ON) - 1);
		}
		else if (strncmp(line, CONF_SOCK_CODE_A_OFF, sizeof(CONF_SOCK_CODE_A_OFF) - 1) == 0)
		{
			conf->sock_code_a_off = atoi(line + sizeof(CONF_SOCK_CODE_A_OFF) - 1);
		}
		else if (strncmp(line, CONF_SOCK_CODE_B_ON, sizeof(CONF_SOCK_CODE_B_ON) - 1) == 0)
		{
			conf->sock_code_b_on = atoi(line + sizeof(CONF_SOCK_CODE_B_ON) - 1);
		}
		else if (strncmp(line, CONF_SOCK_CODE_B_OFF, sizeof(CONF_SOCK_CODE_B_OFF) - 1) == 0)
		{
			conf->sock_code_b_off = atoi(line + sizeof(CONF_SOCK_CODE_B_OFF) - 1);
		}
		else if (strncmp(line, CONF_SOCK_CODE_C_ON, sizeof(CONF_SOCK_CODE_C_ON) - 1) == 0)
		{
			conf->sock_code_c_on = atoi(line + sizeof(CONF_SOCK_CODE_C_ON) - 1);
		}
		else if (strncmp(line, CONF_SOCK_CODE_C_OFF, sizeof(CONF_SOCK_CODE_C_OFF) - 1) == 0)
		{
			conf->sock_code_c_off = atoi(line + sizeof(CONF_SOCK_CODE_C_OFF) - 1);
		}
		if (strncmp(line, CONF_HYGRO_ENABLED, sizeof(CONF_HYGRO_ENABLED) - 1) == 0)
		{
			conf->hygro_enabled = atoi(line + sizeof(CONF_HYGRO_ENABLED) - 1);
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
