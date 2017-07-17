#include "terra_time.h"

#include <ctype.h>

static inline int terra_time_parse_hours_min(terra_time * const time, char const * const line)
{
	ssize_t i;

	if (!isdigit(line[0]))
	{
		terra_log_error("[terra_time_parse_hours_min] not starting with digit\n");
		return -1;
	}

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
	{
		terra_log_error("[terra_time_parse_hours_min] failed to parse hour\n");
		return -1;
	}

	if (isdigit(line[i]) && isdigit(line[i + 1]))
	{
		time->min = atoi(line + i);
	}
	else
	{
		terra_log_error("[terra_time_parse_hours_min] failed to parse min\n");
		return -1;
	}

	if (time->hour > 23 || time->min > 59)
	{
		terra_log_error("[terra_time_parse_hours_min] hour/min out of range\n");
		return -1;
	}

	return i + 2;
}

static inline BOOL terra_time_parse_secs(terra_time * const time, char const * const line)
{
	if (line[0] != ':')
	{
		terra_log_error("[terra_time_parse_secs] missing separator\n");
		return FALSE;
	}

	if (!isdigit(line[1]) && !isdigit(line[2]))
	{
		terra_log_error("[terra_time_parse_secs] missing digits\n");
		return FALSE;
	}

	time->sec = atoi(line + 1);

	if (time->sec > 59)
	{
		terra_log_error("[terra_time_parse_secs] sec out of range\n");
		return FALSE;
	}

	return TRUE;
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
		time->sec = 0;
	}

	return TRUE;

error:
	terra_log_error("[terra_time_parse] failed to parse time (%s)\n", line);
	return FALSE;
}
