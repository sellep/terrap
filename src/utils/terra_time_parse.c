#include "terra_time.h"

#include <ctype.h>

static inline int terra_time_parse_hours_min(terra_time * const time, char const * const str)
{
	ssize_t i;

	if (!isdigit(str[0]))
		return -1;

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
		return -1;
	}

	if (isdigit(str[i]) && isdigit(str[i + 1]))
	{
		time->min = atoi(str + i);
	}
	else
	{
		return -1;
	}

	if (time->hour > 23 || time->min > 59)
	{
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
	{
		printf("ERROR H/M");
		return FALSE;
	}

	if (format == HOUR_MIN_SEC)
	{
		if (!terra_time_parse_secs(time, str + i))
		{
			printf("ERROR S");
			return FALSE;
		}
	}
	else
	{
		time->sec = 0;
	}

	return TRUE;
}
