#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../utils/terra_time.h"
#include "../utils/terra_data.h"

typedef struct
{
	BOOL enabled;
	int pin;
	int repeats;
	int delay;
} terra_conf_hygro;

extern BOOL terra_hygro_read(float * const, float * const, terra_conf_hygro const * const, size_t * const);
extern BOOL terra_hygro_write(float const, float const, terra_time const * const);

static terra_time hygro_last = { 0, 0, 0 };

static inline BOOL hygro_wait(terra_conf_hygro const * const conf, terra_time const * const now, size_t const err_count)
{
	return terra_time_diff(now, &hygro_last) > (conf->delay * (err_count + 1)) ? FALSE : TRUE;
}

static inline BOOL terra_hygro_run(float * const h, float * const t, terra_conf_hygro const * const conf, terra_time const * const now, size_t * const err_count)
{
	BOOL result = terra_hygro_read(h, t, conf, err_count);
	if (LIKELY(result))
	{
		terra_time_cpy(&hygro_last, now);
	}
	return result;
}

#endif
