#ifndef _P_TERRA_DEFS_H
#define _P_TERRA_DEFS_H

#define BOOL int
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common_dht_read.h"
#include "pi_2_mmio.h"
#include "pi_2_dht_read.h"

#define LIKELY(x)       __builtin_expect((x),1)
#define UNLIKELY(x)     __builtin_expect((x),0)

#define OWNER_READ_WRITE 600
#define USER_WRITE 664

#define TERRA_PID_FILE "/var/run/terra.pid"

#endif
