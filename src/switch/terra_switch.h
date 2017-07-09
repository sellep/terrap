#ifndef __P_TERRA_SWITCH_H
#define __P_TERRA_SWITCH_H

#include "../terra_runtime.h"
#include "../utils/terra_time.h"

typedef struct
{
	char sock;
	BOOL set_on;
} terra_switch_req;

BOOL terra_switch_arg(terra_switch_req * const, int const, char const * const * const);
void terra_switch_set(terra_conf const * const, terra_switch_req const * const);

#endif
