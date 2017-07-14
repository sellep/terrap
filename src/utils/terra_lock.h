#ifndef __P_TERRA_LOCK_H
#define __P_TERRA_LOCK_H

#include "../terra_defs.h"
#include "terra_log.h"

extern BOOL terra_lock_init();

pthread_mutex_t *mutex;

#define LOCK() terra_lock();
#define UNLOCK() terra_unlock();

static inline void terra_lock()
{
#ifndef DEBUG
	pthread_mutex_lock(mutex);
#endif
}

static inline void terra_unlock()
{
#ifndef DEBUG
	pthread_mutex_unlock(mutex);
#endif
}

#endif