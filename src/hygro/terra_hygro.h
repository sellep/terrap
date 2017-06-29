#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../terra_defs.h"
#include "../pi_2_mmio.h"
#include "../utils/terra_log.h"
#include "../utils/terra_proc.h"
#include "../utils/terra_time.h"
#include "../conf/terra_conf.h"

#define HYGRO_ERROR_TIMEOUT -1
#define HYGRO_ERROR_CHECKSUM -2
#define HYGRO_SUCCESS 0

typedef struct
{
	BOOL read;
	float humi;
	float temp;
} terra_hygro_cmd;

typedef int hygro_err;

BOOL terra_hygro_arg(terra_hygro_cmd * const, ssize_t const, char const * const * const);
BOOL terra_hygro_run(terra_conf const * const, terra_hygro_cmd const * const);
hygro_err terra_hygro_read(ssize_t const, float * const, float * const);
BOOL terra_hygro_read_rep(terra_conf const * const, float * const, float * const);
BOOL terra_hygro_write(terra_conf const * const, terra_time const * const, float const, float const);

#endif