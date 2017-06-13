#include "terra_time.h"

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
		return terra_time_diff_raw(&_day_end, b) + terra_time_diff_raw(a, &_day_begin);

	return terra_time_diff_raw(a, b);
}

void terra_time_cpy(terra_time * const dest, terra_time const * const src)
{
	dest->hour = src->hour;
	dest->min = src->min;
	dest->sec = src->sec;
}

void terra_time_difft(terra_time * const c, terra_time const * const a, terra_time const * const b)
{
	size_t secs;
	secs = terra_time_diff(a, b);

	c->hour = secs / 60 / 60;
	c->min = (secs - c->hour) / 60;
	c->sec = secs - c->hour - c->min;
}