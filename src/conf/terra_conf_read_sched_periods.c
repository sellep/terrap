#include "terra_conf.h"

#define CONF_SCHED_PERIOD_SECTION_BEGIN "PERIOD SCHEDULE BEGIN"
#define CONF_SCHED_PERIOD_SECTION_END "PERIOD SCHEDULE END"

#define CONF_SCHED_PERIOD_ON_DURATION "\ton_dur="
#define CONF_SCHED_PERIOD_OFF_DURATION "\toff_dur="

extern BOOL terra_conf_read_sched(terra_sched * const, FILE * const);

BOOL terra_conf_read_sched_periods(terra_conf * const conf, FILE * const f)
{
	char *line = NULL;
	size_t buf_len = 0;
	size_t read;
	terra_sched_period *period;

	conf->sched_periods_len = 0;

	while ((read = getline(&line, &buf_len, f)) != -1)
	{
		TRY_CONTINUE(line);

		if (strncmp(line, CONF_SCHED_PERIOD_SECTION_BEGIN, sizeof(CONF_SCHED_PERIOD_SECTION_BEGIN) - 1) != 0)
			continue;

		period = &conf->sched_periods[conf->sched_periods_len];

		if(!terra_conf_read_sched(&period->sched, f))
			return FALSE;

		//schedule period on_dur

		if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of period schedule section\n");

		if (strncmp(line, CONF_SCHED_PERIOD_ON_DURATION, sizeof(CONF_SCHED_PERIOD_ON_DURATION) - 1) == 0)
		{
			if(!terra_time_read(&period->on_dur, line + sizeof(CONF_SCHED_PERIOD_ON_DURATION) - 1)) HANDLE_ERROR("invalid on_dur\n");
		}
		else HANDLE_ERROR("period schedule on_dur expected\n");

		//schedule period off_dur

		if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of period schedule section\n");

		if (strncmp(line, CONF_SCHED_PERIOD_OFF_DURATION, sizeof(CONF_SCHED_PERIOD_OFF_DURATION) - 1) == 0)
		{
			if(!terra_time_read(&period->off_dur, line + sizeof(CONF_SCHED_PERIOD_OFF_DURATION) - 1)) HANDLE_ERROR("invalid off_dur\n");
		}
		else HANDLE_ERROR("period schedule off_dur expected\n");

		//schedule period end

		if ((read = getline(&line, &buf_len, f)) == -1) HANDLE_ERROR("unexpected end of period schedule section\n");

		if (strncmp(line, CONF_SCHED_PERIOD_SECTION_END, sizeof(CONF_SCHED_PERIOD_SECTION_END) - 1) != 0) HANDLE_ERROR("invalid section end\n");

		period->sched.trig = TRIGGER_PERIOD;
		conf->sched_periods_len++;
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
