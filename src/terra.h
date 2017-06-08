#ifndef __P_TERRA_H
#define __P_TERRA_H

#define BOOL int
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/terra_log.h"

#define LED_PIN_ERR 0

#define LOCK()(terra_lock())
#define UNLOCK()(terra_unlock())

void terra_led_set(ssize_t const, BOOL const);

BOOL terra_lock_init();
void terra_lock();
void terra_unlock();

#endif
