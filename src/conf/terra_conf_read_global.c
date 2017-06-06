#include "terra_conf.h"

#define CONF_TRAN_PIN "tran_pin="
#define CONF_TRAN_REP "tran_rep="
#define CONF_SOCK_CHAN "sock_chan="
#define CONF_SOCK_CODE_A_ON "sock_code_a_on="
#define CONF_SOCK_CODE_A_OFF "sock_code_a_off="
#define CONF_SOCK_CODE_B_ON "sock_code_b_on="
#define CONF_SOCK_CODE_B_OFF "sock_code_b_off="
#define CONF_SOCK_CODE_C_ON "sock_code_c_on="
#define CONF_SOCK_CODE_C_OFF "sock_code_c_off="

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

		if (strncmp(line, CONF_TRAN_PIN, sizeof(CONF_TRAN_PIN) - 1) == 0)
		{
			conf->tran_pin = atoi(line + sizeof(CONF_TRAN_PIN));
		}
		else if (strncmp(line, CONF_TRAN_REP, sizeof(CONF_TRAN_REP) - 1) == 0)
		{
			conf->tran_rep = atoi(line + sizeof(CONF_TRAN_REP));
		}
		else if (strncmp(line, CONF_SOCK_CHAN, sizeof(CONF_SOCK_CHAN) - 1) == 0
		{
			conf->sock_chan = atoi(line + sizeof(CONF_SOCK_CHAN));
		}
		else if (strncmp(line, CONF_SOCK_CODE_A_ON, sizeof(CONF_SOCK_CODE_A_ON) - 1) == 0)
		{
			conf->sock_code_a_on = atoi(line + sizeof(CONF_SOCK_CODE_A_ON));
		}
		else if (strncmp(line, CONF_SOCK_CODE_A_OFF, sizeof(CONF_SOCK_CODE_A_OFF) - 1) == 0)
		{
			conf->sock_code_a_off = atoi(line + sizeof(CONF_SOCK_CODE_A_OFF));
		}
		else if (strncmp(line, CONF_SOCK_CODE_B_ON, sizeof(CONF_SOCK_CODE_B_ON) - 1) == 0)
		{
			conf->sock_code_b_on = atoi(line + sizeof(CONF_SOCK_CODE_B_ON));
		}
		else if (strncmp(line, CONF_SOCK_CODE_B_OFF, sizeof(CONF_SOCK_CODE_B_OFF) - 1) == 0)
		{
			conf->sock_code_b_off = atoi(line + sizeof(CONF_SOCK_CODE_B_OFF));
		}
		else if (strncmp(line, CONF_SOCK_CODE_C_ON, sizeof(CONF_SOCK_CODE_C_ON) - 1) == 0)
		{
			conf->sock_code_c_on = atoi(line + sizeof(CONF_SOCK_CODE_C_ON));
		}
		else if (strncmp(line, CONF_SOCK_CODE_C_OFF, sizeof(CONF_SOCK_CODE_C_OFF) - 1))
		{
			conf->sock_code_c_off = atoi(line + sizeof(CONF_SOCK_CODE_C_OFF));
		}
	}

	if (line)
	{
		free(line);
	}

	return TRUE;
}
