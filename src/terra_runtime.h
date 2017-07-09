#ifndef __P_TERRA_RUNTIME_H
#define __P_TERRA_RUNTIME_H

#include "pi_2_mmio.h"
#include "common_dht_read.h"

#include "terra_defs.h"
#include "utils/terra_log.h"
#include "conf/terra_conf.h"

typedef struct
{
	terra_conf conf;
	int hygro_err;
} terra_runtime;

static terra_conf conf;
static terra_runtime runtime;

extern BOOL terra_runtime_init(char const * const);


#endif
