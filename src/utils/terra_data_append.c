#include "terra_data.h"

#include "../terra_defs.h"

#include <time.h>

#define DATA_FORMAT "%02zu:%02zu:%02zu %.2f %.2f\n"

static char _buf[50];

BOOL terra_data_append(terra_data_entry const * const entry)
{
	terra_date date;
	FILE *f;

	terra_date_now(&date, 0);
	sprintf(_buf, DATA_PATH, date.day, date.mon, date.year);

	f = fopen(_buf, "a+");
	if (!f)
	{
		terra_log_error("[terra_data_append] unable to open file %s\n", _buf);
		return FALSE;
	}

	fprintf(f, DATA_FORMAT, entry->tm.hour, entry->tm.min, entry->tm.sec, entry->humi, entry->temp);

	fclose(f);
	return TRUE;
}