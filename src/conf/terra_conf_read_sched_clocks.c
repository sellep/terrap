#include "terra_conf.h"

#define CONF_SCHED_CLOCK_BEGIN "begin clock schedule"

#define CONF_SCHED_CLOCK_START "\tstart="
#define CONF_SCHED_CLOCK_END "\tend="

extern BOOL terra_conf_read_sched(terra_sched * const, FILE * const);

BOOL terra_conf_read_sched_clocks(terra_conf * const conf, FILE * const f)
{
	char *line = NULL;
	size_t buf_len = 0;
	size_t read;
	terra_sched_clock *clock;

	conf->sched_clocks_len = 0;

	while ((read = getline(&line, &buf_len, f)) != -1)
	{
		if (line[0] == '\0')
			continue;
		
		if (line[0] == '#')
			continue;

		if (strncmp(line, CONF_SCHED_CLOCK_BEGIN, sizeof(CONF_SCHED_CLOCK_BEGIN) - 1) != 0)
			continue;

		clock = &conf->sched_clocks[sched_clocks_len];

		if(!terra_conf_read_sched(&clock->sched, f))
			return FALSE;

		//schedule clock start

		if ((read = getline(&line, &buf_len, f)) == -1)
			HANDLE_ERROR("unexpected end of clock schedule section\n");

		if (strncmp(line, CONF_SCHED_CLOCK_START, sizeof(CONF_SCHED_CLOCK_START) - 1) == 0)
		{
			//handle start
		}
		else
			HANDLE_ERROR("clock schedule start expected\n");

		//schedule clock end

		if ((read = getline(&line, &buf_len, f)) == -1)
			HANDLE_ERROR("unexpected end of clock schedule section\n");

		if (strncmp(line, CONF_SCHED_CLOCK_END, sizeof(CONF_SCHED_CLOCK_END) - 1) == 0)
		{
			//handle end
		}
		else
			HANDLE_ERROR("clock schedule end expected\n");

		conf->sched_clocks[conf->sched_clocks_len].sched.trig = TRIGGER_CLOCK;
		conf->sched_clocks_len++;
	}

exit:
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
