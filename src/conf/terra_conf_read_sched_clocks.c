#include "terra_conf.h"

#define HANDLE_ERROR(s){		\
	fprintf(stderr, "%s", s);	\
	goto error;}

#define CONF_SCHED_CLOCK_BEGIN "begin clock schedule"
#define CONF_SCHED_CLOCK_END "end clock schedule"

#define CONF_SCHED_CLOCK_NAME "\tname="
#define CONF_SCHED_CLOCK_SOCK "\tsock="
#define CONF_SCHED_CLOCK_ENABLED "\tenabled="
#define CONF_SCHED_CLOCK_START "\tstart="
#define CONF_SCHED_CLOCK_END "\tend="

BOOL terra_conf_read_sched_clocks(terra_conf * const conf, FILE * const f)
{
	char *line = NULL;
	size_t buf_len = 0;
	size_t read;

	conf->sched_clocks_len = 0;

	while ((read = getline(&line, &buf_len, f)) != -1)
	{
		if (line[0] == '\0')
			continue;
		
		if (line[0] == '#')
			continue;

		if (strncmp(line, CONF_SCHED_CLOCK_BEGIN, sizeof(CONF_SCHED_CLOCK_BEGIN) - 1) != 0)
			continue;

		//schedule name

		if ((read = getline(&line, &buf_len, f)) == -1)
			HANDLE_ERROR("unexpected end of clock schedule section\n");

		if (strncmp(line, CONF_SCHED_CLOCK_NAME, sizeof(CONF_SCHED_CLOCK_NAME) - 1) == 0)
		{
			strncpy(&(conf->sched_clocks[conf->sched_clocks_len].sched.name), line + sizeof(CONF_SCHED_CLOCK_NAME) - 1, read - sizeof(CONF_SCHED_CLOCK_NAME));
			conf->sched_clocks[conf->sched_clocks_len].sched.name[read - sizeof(CONF_SCHED_CLOCK_NAME)] = '\0';
		}
		else
			HANDLE_ERROR("clock schedule name expected\n");

		//schedule sock

		if ((read = getline(&line, &buf_len, f)) == -1)
			HANDLE_ERROR("unexpected end of clock schedule section\n");

		if (strncmp(line, CONF_SCHED_CLOCK_SOCK, sizeof(CONF_SCHED_CLOCK_SOCK) - 1) == 0)
		{
			conf->sched_clocks[conf->sched_clocks_len].sched.sock = line[sizeof(CONF_SCHED_CLOCK_SOCK) - 1];
		}
		else
			HANDLE_ERROR("clock schedule sock expected\n");

		//schedule enabled

		if ((read = getline(&line, &buf_len, f)) == -1)
			HANDLE_ERROR("unexpected end of clock schedule section\n");

		if (strncmp(line, CONF_SCHED_CLOCK_ENABLED, sizeof(CONF_SCHED_CLOCK_ENABLED) - 1) == 0)
		{
			conf->sched_clocks[conf->sched_clocks_len].sched.enabled = atoi(line[sizeof(CONF_SCHED_CLOCK_ENABLED) - 1]);
			if (conf->sched_clocks[conf->sched_clocks_len].sched.enabled > 1)
				HANDLE_ERROR("invalid value for enabled\n");
		}
		else
			HANDLE_ERROR("clock schedule enabled expected\n");

		if (strncmp(line, CONF_SCHED_CLOCK_START, sizeof(CONF_SCHED_CLOCK_START) - 1) == 0)
		{
			//handle start
		}
		else
			HANDLE_ERROR("clock schedule start expected\n");

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
