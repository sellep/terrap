#include "terra_conf.h"

#define CONF_GLOBAL_BEGIN "GLOBAL BEGIN"
#define CONF_GLOBAL_END "GLOBAL END"
#define CONF_TICK "tick="
#define CONF_LED_PIN_ALERT "led_pin_alert="
#define CONF_LED_PIN_HEART "led_pin_heart="
#define CONF_HEART_TICK "heart_tick="
#define CONF_HEART_DUR "heart_dur="
#define CONF_READ_ONLY "read_only="
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
#define CONF_HYGRO_REP "hygro_rep="
#define CONF_HYGRO_WRITE_DELAY "hygro_write_delay="

BOOL terra_conf_read_global(terra_conf * const conf, FILE * const f)
{
	BOOL status = TRUE;

	char *line = NULL;
	size_t buf_len = 0;
	size_t read;

	while ((read = getline(&line, &buf_len, f)) != -1)
	{
		TRY_CONTINUE(line);

		if (strncmp(line, CONF_GLOBAL_BEGIN, sizeof(CONF_GLOBAL_BEGIN) - 1) != 0)
		{
			terra_log_error("[terra_conf_read_global] global begin missing\n");
			return FALSE;
		}

		break;
	}

	while ((read = getline(&line, &buf_len, f)) != -1)
	{
		TRY_CONTINUE(line);

		if (strncmp(line, CONF_TICK, sizeof(CONF_TICK) - 1) == 0)
		{
			conf->tick = atoi(line + sizeof(CONF_TICK) - 1);
		}
		else if (strncmp(line, CONF_LED_PIN_ALERT, sizeof(CONF_LED_PIN_ALERT) - 1) == 0)
		{
			conf->led_pin_alert = atoi(line + sizeof(CONF_LED_PIN_ALERT) - 1);
		}
		else if (strncmp(line, CONF_LED_PIN_HEART, sizeof(CONF_LED_PIN_HEART) - 1) == 0)
		{
			conf->led_pin_heart = atoi(line + sizeof(CONF_LED_PIN_HEART) - 1);
		}
		else if (strncmp(line, CONF_HEART_TICK, sizeof(CONF_HEART_TICK) - 1) == 0)
		{
			conf->heart_tick = atoi(line + sizeof(CONF_HEART_TICK) - 1);
		}
		else if (strncmp(line, CONF_HEART_DUR, sizeof(CONF_HEART_DUR) - 1) == 0)
		{
			conf->heart_dur = atoi(line + sizeof(CONF_HEART_DUR) - 1);
		}
		else if (strncmp(line, CONF_READ_ONLY, sizeof(CONF_READ_ONLY) - 1) == 0)
		{
			conf->read_only = atoi(line + sizeof(CONF_READ_ONLY) - 1);
		}
		else if (strncmp(line, CONF_SWITCH_PIN, sizeof(CONF_SWITCH_PIN) - 1) == 0)
		{
			conf->switch_pin = atoi(line + sizeof(CONF_SWITCH_PIN) - 1);
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
		}
		else if (strncmp(line, CONF_HYGRO_TICK, sizeof(CONF_HYGRO_TICK) - 1) == 0)
		{
			conf->hygro_tick = atoi(line + sizeof(CONF_HYGRO_TICK) - 1);
		}
		else if (strncmp(line, CONF_HYGRO_REP, sizeof(CONF_HYGRO_REP) - 1) == 0)
		{
			conf->hygro_rep = atoi(line + sizeof(CONF_HYGRO_REP) - 1);
		}
		else if (strncmp(line, CONF_HYGRO_WRITE_DELAY, sizeof(CONF_HYGRO_WRITE_DELAY) - 1) == 0)
		{
			if(!terra_time_read(&conf->hygro_write_delay, line + sizeof(CONF_HYGRO_WRITE_DELAY) - 1)) HANDLE_ERROR("[terra_conf_read_global] invalid hygro_write_delay\n");
		}
		else if (strncmp(line, CONF_GLOBAL_END, sizeof(CONF_GLOBAL_END) - 1) == 0)
		{
			break;
		}
		else
		{
			terra_log_error("[terra_conf_read_global] unknown config line: %s\n", line);
			return FALSE;
		}
	}

exit:
	if (line)
	{
		free(line);
	}

	return status;
}
