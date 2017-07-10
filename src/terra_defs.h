#ifndef _P_TERRA_DEFS_H
#define _P_TERRA_DEFS_H

#define BOOL int
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "pi_2_mmio.h"

#define LOCK()(terra_lock())
#define UNLOCK()(terra_unlock())

#define LIKELY(x)       __builtin_expect((x),1)
#define UNLIKELY(x)     __builtin_expect((x),0)

inline static void terra_lock()
{
#ifndef DEBUG
	pthread_mutex_lock();
#endif
}

inline static void terra_unlock()
{
#ifndef DEBUG
	pthread_mutex_unlock();
#endif
}

inline static void terra_pin_out()
{
	LOCK();
#ifndef DEBUG
	pi_2_mmio_set_output(p);
#endif	
	UNLOCK();
}

#endif
