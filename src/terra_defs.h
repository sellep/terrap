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

#define LOCK()(terra_lock())
#define UNLOCK()(terra_unlock())

#define LIKELY(x)       __builtin_expect((x),1)
#define UNLIKELY(x)     __builtin_expect((x),0)

static inline terra_pin_set_out(int const pin)
{
	pi_2_mmio_set_output(pin);
}

static inline void terra_lock()
{
#ifndef DEBUG
	pthread_mutex_lock();
#endif
}

static inline void terra_unlock()
{
#ifndef DEBUG
	pthread_mutex_unlock();
#endif
}

static inline void terra_pin_out()
{
	LOCK();
#ifndef DEBUG
	pi_2_mmio_set_output(p);
#endif	
	UNLOCK();
}

#endif
