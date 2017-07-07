#ifndef __P_TERRA_SWITCH_H
#define __P_TERRA_SWITCH_H

#include "../terra_defs.h"
#include "../common_dht_read.h"
#include "../pi_2_mmio.h"
#include "../utils/terra_log.h"
#include "../utils/terra_lock.h"
#include "../utils/terra_time.h"
#include "../conf/terra_conf.h"

typedef struct
{
	char sock;
	BOOL set_on;
} terra_switch_req;

BOOL terra_switch_arg(terra_switch_req * const, int const, char const * const * const);
void terra_switch_set(terra_conf const * const, terra_switch_req const * const);

#endif
