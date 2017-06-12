#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../terra.h"
#include "../conf/terra_conf.h"

#define HYGRO_ERROR_TIMEOUT -1
#define HYGRO_ERROR_CHECKSUM -2
#define HYGRO_SUCCESS 0

typedef int hygro_err;

hygro_err terra_hygro_read(ssize_t const, float * const, float * const);

BOOL terra_hygro_read_rep(terra_conf const * const, float * const, float * const);

#endif