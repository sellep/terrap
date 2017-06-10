#include "terra_time.h"

void terra_time_cpy(terra_time * const dest, terra_time const * const src)
{
	dest->hour = src->hour;
	dest->min = src->min;
	dest->sec = src->sec;
}
