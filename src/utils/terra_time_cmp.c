#include "terra_time.h"

time_cmp terra_time_cmp(terra_time const * const a, terra_time const * const b)
{
	if (a->hour > b->hour)
		return TIME_ABOVE;
	if (a->hour < b->hour)
		return TIME_BELOW;

	if (a->min > b->min)
		return TIME_ABOVE;
	if (a->min < b->min)
		return TIME_BELOW;

	if (a->sec > b->sec)
		return TIME_ABOVE;
	if (a->sec < b->sec)
		return TIME_BELOW;

	return TIME_EQUAL;
}