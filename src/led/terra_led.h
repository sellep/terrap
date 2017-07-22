#ifndef __P_TERRA_LED_H
#define __P_TERRA_LED_H

#include "../terra_defs.h"
#include "../utils/terra_log.h"
#include "../utils/terra_lock.h"

typedef struct
{
	int err_pin;
	int heart_pin;
	int heart_tick;
	int heart_duration;
} terra_conf_led;

enum led_cmds
{
	LED_NONE = 0,
	LED_ERR = 2,
	LED_HEART = 4,
	LED_ON = 8,
	LED_OFF = 16
};

typedef int terra_led_cmd;

extern terra_led_cmd terra_led_arg(int const, char * * const);

static inline void terra_led_set(ssize_t const pin, terra_led_cmd const set)
{
	LOCK();

	if (set & LED_ON)
	{
#ifndef DEBUG
		pi_2_mmio_set_high(pin);
#endif
	}
	else
	{
#ifndef DEBUG
		pi_2_mmio_set_low(pin);
#endif
	}

	UNLOCK();
}

static inline BOOL terra_led_set_from_cmd(terra_conf_led const * const conf, terra_led_cmd const cmd)
{
	if (cmd == LED_NONE)
		return TRUE;

	if (cmd & LED_ERR)
	{
		terra_led_set(conf->err_pin, cmd);
		return TRUE;
	}
	else if (cmd & LED_HEART)
	{
		terra_led_set(conf->heart_pin, cmd);
		return TRUE;
	}

	terra_log_error("[terra_led_set_from_cmd] missing led target\n");
	return FALSE;
}

#endif
