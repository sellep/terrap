#ifndef __P_TERRA_UTILS_TIME_H
#define __P_TERRA_UTILS_TIME_H

#include "../terra_defs.h"
#include "../utils/terra_log.h"

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
	ssize_t day;
	ssize_t mon;
	ssize_t year;
} terra_date;

typedef struct
{
	terra_time start;
	terra_time stop;
} terra_start_stop;

void terra_date_now(terra_date * const, short const);

time_cmp terra_time_cmp(terra_time const * const, terra_time const * const);
void terra_time_sys(terra_time * const);
void terra_time_print(terra_time const * const);
BOOL terra_time_read(terra_time * const, char const * const);
size_t terra_time_diff(terra_time const * const a, terra_time const * const b);

void sleep_milliseconds(size_t const);
void sleep_microseconds(size_t const);
void busy_wait_milliseconds(size_t const);

void terra_time_cpy(terra_time * const, terra_time const * const);
void terra_time_difft(terra_time * const, terra_time const * const, terra_time const * const);

#define DAY_BEGIN ((terra_time) { 0, 0, 0 })
#define DAY_END ((terra_time) { 23, 59, 59 })

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

#endif
