#include "terra_time.h"

#include <time.h>

void terra_time_sys(terra_time * const tim)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	tim->hour = tm.tm_hour;
	tim->min = tm.tm_min;
	tim->sec = tm.tm_sec;
}
