#ifndef __P_TERRA_UTILS_MODE_H
#define __P_TERRA_UTILS_MODE_H

#include "../terra_defs.h"

#define MODE_NAME_MAX 20

typedef char* terra_mode;

extern BOOL terra_mode_run(int const, char * * const);
extern BOOL terra_mode_read(terra_mode * const);

#endif
