#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../terra_runtime.h"

static terra_time hygro_last = { 0, 0, 0 };

extern BOOL terra_hygro_run(float * const, float * const);
extern BOOL terra_hygro_write(float const, float const);

inline static BOOL hygro_wait()
{
	return terra_time_diff(&runtime.now, &hygro_last) > conf.hygro_delay_tick ? FALSE : TRUE;
}

#endif