#include "terra_time.h"

static terra_time time_end = { 24, 0, 0 };
static terra_time time_begin = { 0, 0, 0 };

inline size_t terra_time_diff_raw(terra_time const * const a, terra_time const * const b)
{
	size_t secs_a;
	size_t secs_b;

	secs_a = a->hour * 60 * 60;
	secs_a += a->min * 60;
	secs_a += a->sec;

	secs_b = b->hour * 60 * 60;
	secs_b += b->min * 60;
	secs_b += b->sec;

	return secs_a - secs_b;
}

size_t terra_time_diff(terra_time const * const a, terra_time const * const b)
{
	if (terra_time_cmp(a, b) == TIME_BELOW)
		return terra_time_diff_raw(&time_end, b) + terra_time_diff_raw(a, &time_begin);

	return terra_time_diff_raw(a, b);
}