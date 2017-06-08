#ifndef __P_TERRA_H
#define __P_TERRA_H

#define BOOL int
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/terra_log.h"

enum terra_lock_targets
{
	TERRA_SWITCH = 1,
	TERRA_HYGRO = 2
};

enum terra_lock_modes
{
	TERRA_LOCK = 128,
	TERRA_RELEASE = 256
};

typedef int terra_lock_mode;

void terra_lock(terra_lock_mode const);

#endif