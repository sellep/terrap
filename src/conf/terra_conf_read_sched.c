#include "terra_conf.h"

#define CONF_SCHED_CLOCK_NAME "\tname="
#define CONF_SCHED_CLOCK_SOCK "\tsock="
#define CONF_SCHED_CLOCK_ENABLED "\tenabled="

BOOL terra_conf_read_sched(terra_sched * const sched, FILE * const f)
{
	char *line = NULL;
	size_t buf_len = 0;
	size_t read;

	//schedule name

	if ((read = getline(&line, &buf_len, f)) == -1)
		HANDLE_ERROR("unexpected end of clock schedule section\n");

	if (strncmp(line, CONF_SCHED_CLOCK_NAME, sizeof(CONF_SCHED_CLOCK_NAME) - 1) == 0)
	{
		strncpy(sched->name, line + sizeof(CONF_SCHED_CLOCK_NAME) - 1, read - sizeof(CONF_SCHED_CLOCK_NAME));
		sched->name[read - sizeof(CONF_SCHED_CLOCK_NAME)] = '\0';
	}
	else
		HANDLE_ERROR("clock schedule name expected\n");

	//schedule sock

	if ((read = getline(&line, &buf_len, f)) == -1)
		HANDLE_ERROR("unexpected end of clock schedule section\n");

	if (strncmp(line, CONF_SCHED_CLOCK_SOCK, sizeof(CONF_SCHED_CLOCK_SOCK) - 1) == 0)
	{
		sched->sock = line[sizeof(CONF_SCHED_CLOCK_SOCK) - 1];
	}
	else
		HANDLE_ERROR("clock schedule sock expected\n");

	//schedule enabled

	if ((read = getline(&line, &buf_len, f)) == -1)
		HANDLE_ERROR("unexpected end of clock schedule section\n");

	if (strncmp(line, CONF_SCHED_CLOCK_ENABLED, sizeof(CONF_SCHED_CLOCK_ENABLED) - 1) == 0)
	{
		sched->enabled = atoi(&line[sizeof(CONF_SCHED_CLOCK_ENABLED) - 1]);
		if (sched->enabled > 1)
			HANDLE_ERROR("invalid value for enabled\n");
	}
	else
		HANDLE_ERROR("clock schedule enabled expected\n");
}