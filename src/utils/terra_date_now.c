#include "terra_time.h"

#include <time.h>

void terra_date_now(terra_date * const date, short const doff)
{
	time_t t = time(NULL);
	struct tm ts = *localtime(&t);
	ts.tm_mday += doff;
	mktime(&ts);

	date->day = ts.tm_mday;
	date->mon = ts.tm_mon + 1;
	date->year = ts.tm_year + 1900;
}