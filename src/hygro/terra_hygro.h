#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../terra_runtime.h"

extern BOOL terra_hygro_run(float * const, float * const);
extern BOOL terra_hygro_write(float const, float const);

static inline BOOL hygro_wait()
{
	return terra_time_diff(&runtime.now, &runtime.hygro_last) > CONF_HYGRO.delay ? FALSE : TRUE;
}

#endif