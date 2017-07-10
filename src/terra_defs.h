#ifndef _P_TERRA_DEFS_H
#define _P_TERRA_DEFS_H

#define BOOL int
#define TRUE 1
#define FALSE 0

enum terra_switch_modes
{
	SWITCH_UNKNOWN = -1,
	SWITCH_OFF  = 0,
	SWITCH_ON = 1
};

typedef int terra_switch_mode;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "pi_2_mmio.h"

#define LIKELY(x)       __builtin_expect((x),1)
#define UNLIKELY(x)     __builtin_expect((x),0)

#endif
