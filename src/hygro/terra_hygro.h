#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../conf/terra_conf.h"
#include "../utils/terra_time.h"
#include "../utils/terra_data.h"

extern BOOL terra_hygro_read(float * const, float * const, terra_conf_hygro const * const);
extern BOOL terra_hygro_write(float const, float const, terra_time const * const);

static terra_time hygro_last = { 0, 0, 0 };

static inline BOOL hygro_wait(terra_conf_hygro const * const conf, terra_time const * const now)
{
	return terra_time_diff(now, &hygro_last) > conf->delay ? FALSE : TRUE;
}

static inline BOOL terra_hygro_run(float * const h, float * const t, terra_conf_hygro const * const conf, terra_time const * const now)
{
	BOOL result = terra_hygro_read(h, t, conf);
	if (result)
	{
		terra_time_cpy(&hygro_last, now);
	}
	return result;
}

#endif