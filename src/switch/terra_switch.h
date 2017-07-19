#ifndef __P_TERRA_SWITCH_H
#define __P_TERRA_SWITCH_H

#include "../terra_defs.h"
#include "../conf/terra_conf.h"
#include "../utils/terra_log.h"
#include "../utils/terra_lock.h"

enum terra_switch_modes
{
	SWITCH_UNKNOWN = -1,
	SWITCH_OFF  = 0,
	SWITCH_ON = 1
};

typedef int terra_switch_mode;

typedef struct
{
	char socket;
	terra_switch_mode mode;
} terra_switch_req;

extern BOOL terra_switch_arg(terra_switch_req * const, int const, char const * const * const);
extern void terra_switch_transmit(terra_conf_switch const * const, int const);

static inline void terra_switch_set_on(terra_conf_switch const * const conf, char const socket, char const * const sender)
{
	if (socket == 'a')
	{
		terra_switch_transmit(conf, conf->code_aon);
	}
	else if (socket == 'b')
	{
		terra_switch_transmit(conf, conf->code_bon);
	}
	else
	{
		terra_switch_transmit(conf, conf->code_con);
	}

	terra_log_info("[terra_switch] set switch %c to on (%s)\n", socket, sender);
}

static inline void terra_switch_set_off(terra_conf_switch const * const conf, char const socket, char const * const sender)
{
	if (socket == 'a')
	{
		terra_switch_transmit(conf, conf->code_aoff);
	}
	else if (socket == 'b')
	{
		terra_switch_transmit(conf, conf->code_boff);
	}
	else
	{
		terra_switch_transmit(conf, conf->code_coff);
	}

	terra_log_info("[terra_switch] set switch %c to off (%s)\n", socket, sender);
}

static inline void terra_switch_set(terra_conf_switch const * const conf, char const socket, terra_switch_mode const mode, char const * const sender)
{
	if (mode == SWITCH_ON)
	{
		terra_switch_set_on(conf, socket, sender);
	}
	else if (mode == SWITCH_OFF)
	{
		terra_switch_set_off(conf, socket, sender);
	}
}

#endif
