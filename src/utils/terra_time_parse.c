#include "terra_time.h"

#include <ctype.h>

static inline int terra_time_parse_hours_min(terra_time * const time, char const * const line)
{
	ssize_t i;

	if (!isdigit(line[0]))
		goto error;

	if (line[1] == ':')
	{
		time->hour = atoi(line);
		i = 2;
	}
	else if (isdigit(line[1]) && line[2] == ':')
	{
		time->hour = atoi(line);
		i = 3;
	}
	else
		goto error;

	if (isdigit(line[i]) && isdigit(line[i + 1]))
	{
		time->min = atoi(line + i);
	}

	if (time->hour > 23 || time->min > 59)
		goto error;

	return i + 2;

error:
	terra_log_error("[terra_time_parse_hours_min] failed to parse time\n");
	return -1;
}

static inline BOOL terra_time_parse_secs(terra_time * const time, char const * const line)
{
	if (line[0] != ':')
		goto error;

	if (!isdigit(line[1]) && !isdigit(line[2]))
		goto error;

	time->sec = atoi(line + 1);

	if (time->sec > 59)
		goto error;

error:
	terra_log_error("[terra_time_parse_secs] failed to parse time\n");
	return -1;
}

BOOL terra_time_parse(terra_time * const time, char const * const line, time_format const format)
{
	ssize_t i;

	if ((i = terra_time_parse_hours_min(time, line)) < 0)
		goto error;

	if (format == HOUR_MIN_SEC)
	{
		if (!terra_time_parse_secs(time, line + i))
			goto error;
	}
	else
	{
		time->secs = 0;
	}

	return TRUE;

error:
	terra_log_error("[terra_time_parse] failed to parse time\n");
	return FALSE;
}
