#ifndef __P_TERRA_UTILS_TIME_H
#define __P_TERRA_UTILS_TIME_H

#include "../terra.h"

enum time_compares {
	TIME_BELOW = -1,
	TIME_EQUAL = 0,
	TIME_ABOVE = 1
};

typedef int time_cmp;

typedef struct
{
	ssize_t hour;
	ssize_t min;
	ssize_t sec;
} terra_time;

typedef struct
{
	terra_time start;
	terra_time stop;
} terra_start_stop;

time_cmp terra_time_cmp(terra_time const * const, terra_time const * const);
void terra_time_sys(terra_time * const);
void terra_time_print(terra_time const * const);
BOOL terra_time_read(terra_time * const, char const * const);
size_t terra_time_diff(terra_time const * const a, terra_time const * const b);

void sleep_milliseconds(size_t const);
void sleep_microseconds(size_t const);
void busy_wait_milliseconds(size_t const);

/*
	inline functions
*/

inline void terra_time_cpy(terra_time * const dest, terra_time const * const src)
{
	dest->hour = src->hour;
	dest->min = src->min;
	dest->sec = src->sec;
}

inline void terra_time_difft(terra_time * const c, terra_time const * const a, terra_time const * const b)
{
	size_t secs;
	secs = terra_time_diff(a, b);

	c->hour = secs / 60 / 60;
	c->min = (secs - c->hour) / 60;
	c->sec = secs - c->hour - c->min;
}

#endif
