#ifndef __P_TERRA_UTILS_TIME_H
#define __P_TERRA_UTILS_TIME_H

#include "../terra.h"

typedef struct
{
	ssize_t hour;
	ssize_t min;
	ssize_t sec;
} terra_time;

void terra_time_print(terra_time const * const);
BOOL terra_time_read(terra_time * const, char const * const);

#endif