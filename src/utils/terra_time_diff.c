#include "terra_time.h"

#define DIFF(a,b)(a>b?a-b:b-a)

size_t terra_time_diff(terra_time const * const a, terra_time const * const b)
{
	size_t secs_a;
	size_t secs_b;

	secs_a = a->hour * 60 * 60;
	secs_a += a->min * 60;
	secs_a += a->sec;

	secs_b = b->hour * 60 * 60;
	secs_b += b->min * 60;
	secs_b += b->sec;

	return DIFF(secs_a, secs_b);
}