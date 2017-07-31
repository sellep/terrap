#include "terra_time.h"

#include <ctype.h>

static inline int terra_time_parse_hours_min(terra_time * const time, char const * const str)
{
	ssize_t i;

	if (!isdigit(str[0]))
	{
		terra_log_error("[terra_time_parse_hours_min] not starting with digit\n");
		return -1;
	}

	if (str[1] == ':')
	{
		time->hour = atoi(str);
		i = 2;
	}
	else if (isdigit(str[1]) && str[2] == ':')
	{
		time->hour = atoi(str);
		i = 3;
	}
	else
	{
		terra_log_error("[terra_time_parse_hours_min] failed to parse hour\n");
		return -1;
	}

	if (isdigit(str[i]) && isdigit(str[i + 1]))
	{
		time->min = atoi(str + i);
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

static inline BOOL terra_time_parse_secs(terra_time * const time, char const * const str)
{
	if (str[0] != ':')
		return FALSE;

	if (!isdigit(str[1]) && !isdigit(str[2]))
		return FALSE;

	time->sec = atoi(str + 1);

	if (time->sec > 59)
		return FALSE;

	return TRUE;
}

BOOL terra_time_parse(terra_time * const time, char const * const str, time_format const format)
{
	printf("try to parse time: %s\n", str);
	ssize_t i;

	if ((i = terra_time_parse_hours_min(time, str)) < 0)
		goto error;

	if (format == HOUR_MIN_SEC)
	{
		if (!terra_time_parse_secs(time, str + i))
			goto error;
	}
	else
	{
		time->sec = 0;
	}

	return TRUE;

error:
	return FALSE;
}
