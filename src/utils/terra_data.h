#ifndef __P_TERRA_DATA_H
#define __P_TERRA_DATA_H

#include "terra_log.h"
#include "terra_time.h"

#define DATA_PATH "/var/lib/terra/data_%02zu.%02zu.%zu"

typedef struct
{
	terra_time tm;
	float humi;
	float temp;
} terra_data_entry;

BOOL terra_data_append(terra_data_entry const * const);
BOOL terra_data_read(terra_data_entry * * const, size_t * const, char * const, ssize_t const);

#endif