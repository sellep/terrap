#include "terra_conf.h"

#define CONF_SCHED_CLOCK_SECTION_BEGIN "CLOCK SCHEDULE BEGIN"
#define CONF_SCHED_CLOCK_SECTION_END "CLOCK SCHEDULE END"

#define CONF_SCHED_CLOCK_TIME_START "\tstart="
#define CONF_SCHED_CLOCK_TIME_STOP "\tstop="

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
		TRY_CONTINUE(line);

		if (strncmp(line, CONF_SCHED_CLOCK_SECTION_BEGIN, sizeof(CONF_SCHED_CLOCK_SECTION_BEGIN) - 1) != 0)
			continue;

		clock = &conf->sched_clocks[conf->sched_clocks_len];

		if(!terra_conf_read_sched(&clock->sched, f))
			return FALSE;

		//schedule clock times

		for (clock->times_len = 0; clock->times_len < TERRA_CONF_MAX_SCHED_CLOCK_TIMES; clock->times_len++)
		{
			if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of clock schedule section\n");

			if (strncmp(line, CONF_SCHED_CLOCK_SECTION_END, sizeof(CONF_SCHED_CLOCK_SECTION_END) - 1) != 0)
				break;

			if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of clock schedule section\n");

			//schedule clock time start

			if (strncmp(line, CONF_SCHED_CLOCK_TIME_START, sizeof(CONF_SCHED_CLOCK_TIME_START) - 1) == 0)
			{
				if(!terra_time_read(&(clock->times[clock->times_len].start), line + sizeof(CONF_SCHED_CLOCK_TIME_START) - 1)) HANDLE_ERROR("invalid start time\n");
			}
			else HANDLE_ERROR("clock schedule start expected\n");

			if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of clock schedule section\n");

			if (strncmp(line, CONF_SCHED_CLOCK_TIME_STOP, sizeof(CONF_SCHED_CLOCK_TIME_STOP) - 1) == 0)
			{
				if(!terra_time_read(&(clock->times[clock->times_len].stop), line + sizeof(CONF_SCHED_CLOCK_TIME_STOP) - 1)) HANDLE_ERROR("invalid end time\n");
		}
		else HANDLE_ERROR("clock schedule end expected\n");

		conf->sched_clocks[conf->sched_clocks_len].sched.trig = TRIGGER_CLOCK;
		conf->sched_clocks_len++;
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
