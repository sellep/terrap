#include "terra_conf.h"

#define CONF_SCHED_TEMP_SECTION_BEGIN "TEMP SCHEDULE BEGIN"
#define CONF_SCHED_TEMP_SECTION_END "TEMP SCHEDULE END"

#define CONF_SCHED_TEMP_ACTIVATION_LIMIT "\tact_lim="
#define CONF_SCHED_TEMP_DEACTIVATION_LIMIT "\tdeact_lim="
#define CONF_SCHED_TEMP_INVERT "\tinvert="

extern BOOL terra_conf_read_sched(terra_sched * const, FILE * const);

BOOL terra_conf_read_sched_temps(terra_conf * const conf, FILE * const f)
{
	char *line = NULL;
	size_t buf_len = 0;
	size_t read;
	terra_sched_temp *temp;

	conf->sched_temps_len = 0;

	while ((read = getline(&line, &buf_len, f)) != -1)
	{
		TRY_CONTINUE(line);

		if (strncmp(line, CONF_SCHED_TEMP_SECTION_BEGIN, sizeof(CONF_SCHED_TEMP_SECTION_BEGIN) - 1) != 0)
			continue;

		temp = &conf->sched_temps[conf->sched_temps_len];

		if(!terra_conf_read_sched(&temp->sched, f))
			return FALSE;

		//schedule temp act_lim

		if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of temp schedule section\n");

		if (strncmp(line, CONF_SCHED_TEMP_ACTIVATION_LIMIT, sizeof(CONF_SCHED_TEMP_ACTIVATION_LIMIT) - 1) == 0)
		{
			temp->act_lim = atof(&line[sizeof(CONF_SCHED_TEMP_ACTIVATION_LIMIT) - 1]);
		}
		else HANDLE_ERROR("temp schedule act_lim expected\n");

		//schedule temp deact_lim

		if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of temp schedule section\n");

		if (strncmp(line, CONF_SCHED_TEMP_DEACTIVATION_LIMIT, sizeof(CONF_SCHED_TEMP_DEACTIVATION_LIMIT) - 1) == 0)
		{
			temp->deact_lim = atof(&line[sizeof(CONF_SCHED_TEMP_DEACTIVATION_LIMIT) - 1]);
		}
		else HANDLE_ERROR("temp schedule deact_lim expected\n");

		//schedule temp invert

		if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of temp schedule section\n");

		if (strncmp(line, CONF_SCHED_TEMP_INVERT, sizeof(CONF_SCHED_TEMP_INVERT) - 1) == 0)
		{
			temp->invert = atoi(&line[sizeof(CONF_SCHED_TEMP_INVERT) - 1]);
		}
		else HANDLE_ERROR("temp schedule invert expected\n");

		//schedule temp end

		if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of temp schedule section\n");

		if (strncmp(line, CONF_SCHED_TEMP_SECTION_END, sizeof(CONF_SCHED_TEMP_SECTION_END) - 1) != 0) HANDLE_ERROR("invalid section end\n");

		temp->sched.trig = TRIGGER_TEMP;
		conf->sched_temps_len++;
	}

	if (line)
	{
		free(line);
	}
	return TRUE;

error:
	if (line)
	{
		free(line);
	}
	return FALSE;
}
