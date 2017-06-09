#ifndef __P_TERRA_UTILS_TIME_H
#define __P_TERRA_UTILS_TIME_H

#include "../terra.h"

enum time_compares {
	TIME_BELOW = -1,
	TIME_EQUAL = 0,
	TIME_ABOVE = 1
};

typedef int time_compare;

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

time_compare terra_time_cmp(terra_time const * const, terra_time const * const);
void terra_time_sleep(size_t const);
void terra_time_sys(terra_time * const);
void terra_time_print(terra_time const * const);
BOOL terra_time_read(terra_time * const, char const * const);
size_t terra_time_diff(terra_time const * const, terra_time const * const);

#endif