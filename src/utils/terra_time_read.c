#include "terra_time.h"

#include <ctype.h>

BOOL terra_time_read(terra_time * const time, char const * const line)
{
	if (!isdigit(line[0]))
		goto error;
	if (!isdigit(line[1]))
		goto error;
	if (line[2] != ':')
		goto error;
	if (!isdigit(line[3]))
		goto error;
	if (!isdigit(line[4]))
		goto error;
	if (line[5] != ':')
		goto error;
	if (!isdigit(line[6]))
		goto error;
	if (!isdigit(line[7]))
		goto error;

	time->hour = atoi(line);
	if (time->hour > 23)
		goto error;

	time->min = atoi(line + 3);
	if (time->min > 59)
		goto error;

	time->sec = atoi(line + 6);
	if (time->sec > 59)
		goto error;

	return TRUE;

error:
	terra_log_error("failed to read time\n");
	return FALSE;
}
