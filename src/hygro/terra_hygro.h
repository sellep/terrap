#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../terra.h"
#include "../conf/terra_conf.h"

#define HYGRO_ERROR_TIMEOUT -1
#define HYGRO_ERROR_CHECKSUM -2
#define HYGRO_SUCCESS 0

typedef int hygro_err;

hygro_err terra_hygro_read(ssize_t const, float * const, float * const);

inline BOOL terra_hygro_read_rep(terra_conf const * const conf, float * const h, float * const t)
{
	ssize_t r;

	for (r = 0; r < conf->hygro_rep; r++)
	{
		if (terra_hygro_read(conf->hygro_pin_io, h, t) == HYGRO_SUCCESS)
		{
			terra_log_info("hygro humidity: %f.2, temperature: %f.2\n", *h, *t);
			return TRUE;
		}
	}

	return FALSE;
}

#endif