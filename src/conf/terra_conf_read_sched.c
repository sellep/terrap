#include "terra_conf.h"

#define CONF_SCHED_NAME "\tname="
#define CONF_SCHED_SOCK "\tsock="
#define CONF_SCHED_ENABLED "\tenabled="

BOOL terra_conf_read_sched(terra_sched * const sched, FILE * const f)
{
	char *line = NULL;
	size_t buf_len = 0;
	size_t read;

	//schedule name

	if ((read = getline(&line, &buf_len, f)) == -1)
		HANDLE_ERROR("unexpected end of schedule section\n");

	if (strncmp(line, CONF_SCHED_NAME, sizeof(CONF_SCHED_NAME) - 1) == 0)
	{
		strncpy(sched->name, line + sizeof(CONF_SCHED_NAME) - 1, read - sizeof(CONF_SCHED_NAME));
		sched->name[read - sizeof(CONF_SCHED_NAME)] = '\0';
	}
	else
		HANDLE_ERROR("schedule name expected\n");

	//schedule sock

	if ((read = getline(&line, &buf_len, f)) == -1)
		HANDLE_ERROR("unexpected end of schedule section\n");

	if (strncmp(line, CONF_SCHED_SOCK, sizeof(CONF_SCHED_SOCK) - 1) == 0)
	{
		sched->sock = line[sizeof(CONF_SCHED_SOCK) - 1];
	}
	else
		HANDLE_ERROR("schedule sock expected\n");

	//schedule enabled

	if ((read = getline(&line, &buf_len, f)) == -1)
		HANDLE_ERROR("unexpected end of schedule section\n");

	if (strncmp(line, CONF_SCHED_ENABLED, sizeof(CONF_SCHED_ENABLED) - 1) == 0)
	{
		sched->enabled = atoi(&line[sizeof(CONF_SCHED_ENABLED) - 1]);
		if (sched->enabled > 1)
			HANDLE_ERROR("invalid value for enabled\n");
	}
	else
		HANDLE_ERROR("schedule enabled expected\n");

	//cleanup

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
