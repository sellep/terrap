#ifndef __P_TERRA_H
#define __P_TERRA_H

#define BOOL int
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/terra_log.h"

#define LOCK()(terra_lock())
#define UNLOCK()(terra_unlock())

BOOL terra_lock_init();
void terra_lock();
void terra_unlock();

#endif