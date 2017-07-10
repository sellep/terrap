#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../terra_runtime.h"

typedef struct
{
	BOOL read;
	float humi;
	float temp;
} terra_hygro_cmd;

typedef int hygro_err;

static terra_time hygro_last = { 0, 0, 0 };

extern BOOL terra_hygro_arg(terra_hygro_cmd * const, ssize_t const, char const * const * const);
extern BOOL terra_hygro_run(terra_hygro_cmd const * const);
extern BOOL terra_hygro_write(float const, float const);

inline static BOOL hygro_wait()
{
	return terra_time_diff(&runtime.now, &hygro_last) > conf.hygro_delay_tick ? FALSE : TRUE;
}

#endif