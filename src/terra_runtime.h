#ifndef __P_TERRA_RUNTIME_H
#define __P_TERRA_RUNTIME_H

#include "pi_2_mmio.h"
#include "common_dht_read.h"
#include "pi_2_dht_read.h"

#include "terra_defs.h"
#include "utils/terra_log.h"
#include "utils/terra_time.h"
#include "conf/terra_conf.h"

typedef struct
{
	BOOL daemon;
	uint64_t tick;
	terra_time now;
	int hygro_err;
} terra_runtime;

static terra_conf conf;
static terra_runtime runtime;

extern BOOL terra_runtime_init(char const * const, BOOL const);

inline static void terra_runtime_tick()
{
	runtime.tick++;
	terra_time_now(&runtime.now);
}

#endif
