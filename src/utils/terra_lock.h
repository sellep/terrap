#ifndef __P_TERRA_UTILS_LOCK_H
#define __P_TERRA_UTILS_LOCK_H

#include <pthread.h>

#include "../terra_defs.h"
#include "terra_log.h"

#define LOCK()(terra_lock())
#define UNLOCK()(terra_unlock())

BOOL terra_lock_init();

extern volatile pthread_mutex_t *_mutex;

static inline void terra_lock()
{
#ifndef DEBUG
	pthread_mutex_lock(_mutex);
#endif
}

static inline void terra_unlock()
{
#ifndef DEBUG
	pthread_mutex_unlock(_mutex);
#endif
}

#endif