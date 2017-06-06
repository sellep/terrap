#include "terra_conf.h"

#define CONF_SCHED_CLOCK_BEGIN "begin clock schedule"
#define CONF_SCHED_CLOCK_END "end clock schedule"

#define CONF_SCHED_CLOCK_NAME "\tname"
#define CONF_SCHED_CLOCK_SOCK "\tsock"

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

		if ((read = getline(&line, &buf_len, f)) == -1)
		{
			fprintf(stderr, "unexpected end of clock schedule section\n");
			goto error;
		}

		if (strncmp(line, CONF_SCHED_CLOCK_NAME, sizeof(CONF_SCHED_CLOCK_NAME) - 1) == 0 && line[sizeof(CONF_SCHED_CLOCK_NAME) - 1] == '=')
		{
			strncpy(&(conf->sched_clocks[conf->sched_clocks_len].sched.name), line + sizeof(CONF_SCHED_CLOCK_NAME), read - sizeof(CONF_SCHED_CLOCK_NAME) - 1);
		}
		else
		{
			fprintf(stderr, "clock schedule name expected\n");
			goto error;
		}

		if (strncmp(line, CONF_SCHED_CLOCK_SOCK, sizeof(CONF_SCHED_CLOCK_SOCK) - 1) == 0 && line[sizeof(CONF_SCHED_CLOCK_SOCK) - 1] == '=')
		{
			conf->sched_clocks[conf->sched_clocks_len].sched.sock = line[sizeof(CONF_SCHED_CLOCK_SOCK)];
		}
		else
		{
			fprintf(stderr, "clock schedule sock expected\n");
			goto error;
		}

		conf->sched_clocks[conf->sched_clocks_len].sched.trig = TRIGGER_CLOCK;
		conf->sched_clocks_len++;
		break;
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
