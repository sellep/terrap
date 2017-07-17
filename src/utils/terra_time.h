#ifndef __P_TERRA_UTILS_TIME_H
#define __P_TERRA_UTILS_TIME_H

#include <time.h>

#include "../terra_defs.h"
#include "../utils/terra_log.h"

enum time_compares {
	TIME_BELOW = -1,
	TIME_EQUAL = 0,
	TIME_ABOVE = 1
};

enum time_formats {
	HOUR_MIN,
	HOUR_MIN_SEC
};

typedef int time_cmp;
typedef int time_format;

typedef struct
{
	ssize_t hour;
	ssize_t min;
	ssize_t sec;
} terra_time;

typedef struct
{
	ssize_t day;
	ssize_t mon;
	ssize_t year;
} terra_date;

typedef struct
{
	terra_time start;
	terra_time stop;
} terra_start_stop;

static terra_time day_begin = { 0, 0, 0 };
static terra_time day_end = { 23, 59, 59 };

extern void terra_time_printnl(terra_time const * const);
extern BOOL terra_time_parse(terra_time * const, char const * const, time_format const);

static inline void terra_time_print(terra_time const * const t)
{
	terra_time_printnl(t);
	printf("\n");
}

static inline void terra_date_now(terra_date * const date, short const doff)
{
	time_t t = time(NULL);
	struct tm ts = *localtime(&t);
	ts.tm_mday += doff;
	mktime(&ts);

	date->day = ts.tm_mday;
	date->mon = ts.tm_mon + 1;
	date->year = ts.tm_year + 1900;
}

static inline void terra_time_cpy(terra_time * const dest, terra_time const * const src)
{
	dest->hour = src->hour;
	dest->min = src->min;
	dest->sec = src->sec;
}

static inline time_cmp terra_time_cmp(terra_time const * const a, terra_time const * const b)
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

static inline size_t terra_time_to_int(terra_time const * const tt)
{
	return tt->hour * 60 * 60 + tt->min * 60 + tt->sec;
}

static inline ssize_t terra_time_to_arr(char * const buf, terra_time const * const tt)
{
	return sprintf(buf, "%zu:%02zu:%02zu", tt->hour, tt->min, tt->sec);
}

static inline void terra_time_from_int(terra_time * const tt, size_t const val)
{
	size_t copy = val;

	tt->hour = copy / 60 / 60;
	copy -= tt->hour * 60 * 60;
	tt->min = copy / 60;
	copy -= tt->min * 60;
	tt->sec = copy;
}

static inline BOOL terra_time_between(terra_time const * const tim, terra_time const * const begin, terra_time const * const end)
{
	if (terra_time_cmp(begin, end) == TIME_BELOW)
	{
		if (terra_time_cmp(tim, begin) != TIME_ABOVE)
			return FALSE;

		if (terra_time_cmp(tim, end) != TIME_BELOW)
			return FALSE;

		return TRUE;
	}

	if (terra_time_cmp(tim, begin) == TIME_ABOVE)
		return TRUE;

	if (terra_time_cmp(tim, end) == TIME_BELOW)
		return TRUE;

	return FALSE;
}

static inline void terra_time_now(terra_time * const tim)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	tim->hour = tm.tm_hour;
	tim->min = tm.tm_min;
	tim->sec = tm.tm_sec;
}

static inline size_t terra_time_diff_raw(terra_time const * const a, terra_time const * const b)
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

static inline size_t terra_time_diff(terra_time const * const a, terra_time const * const b)
{
	if (terra_time_cmp(a, b) == TIME_BELOW)
		return terra_time_diff_raw(&day_end, b) + terra_time_diff_raw(a, &day_begin);

	return terra_time_diff_raw(a, b);
}

static inline void terra_time_difft(terra_time * const c, terra_time const * const a, terra_time const * const b)
{
	size_t secs = terra_time_diff(a, b);

	c->hour = secs / 60 / 60;
	secs -= c->hour * 60 * 60;
	c->min = secs / 60;
	c->sec -= secs - c->min * 60;
}

#endif
