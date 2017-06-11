#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../terra.h"

#define HYGRO_ERROR_TIMEOUT -1
#define HYGRO_ERROR_CHECKSUM -2
#define HYGRO_ERROR_ARGUMENT -3
#define HYGRO_ERROR_GPIO -4
#define HYGRO_SUCCESS 0

typedef int hygro_err;

hygro_err terra_hygro_read(ssize_t const, float * const, float * const);

#endif