#ifndef __P_TERRA_SWITCH_H
#define __P_TERRA_SWITCH_H

#include "../terra_runtime.h"

typedef struct
{
	char socket;
	terra_switch_mode mode;
} terra_switch_req;

#define SWITCH_NOT_ON(s) ((terra_scheduler*)(s))->state != SWITCH_ON
#define SWITCH_NOT_OFF(s) ((terra_scheduler*)(s))->state != SWITCH_OFF

BOOL terra_switch_arg(terra_switch_req * const, int const, char const * const * const);
void terra_switch_set(terra_switch_req const * const);

#endif
