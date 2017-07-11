#ifndef __P_TERRA_SWITCH_H
#define __P_TERRA_SWITCH_H

#include "../terra_runtime.h"

typedef struct
{
	char socket;
	terra_switch_mode mode;
} terra_switch_req;

extern BOOL terra_switch_arg(terra_switch_req * const, int const, char const * const * const);
extern void terra_switch_transmit(int const);

static inline void terra_switch_on(char const socket)
{
	if (socket == 'a')
	{
		terra_switch_transmit(CONF_SWITCH.code_aon);
	}
	else if (socket == 'b')
	{
		terra_switch_transmit(CONF_SWITCH.code_bon);
	}
	else
	{
		terra_switch_transmit(CONF_SWITCH.code_con);
	}

	terra_log_info("[terra_switch] set switch %c to on\n", socket);
}

static inline void terra_switch_off(char const socket)
{
	if (socket == 'a')
	{
		terra_switch_transmit(CONF_SWITCH.code_aoff);
	}
	else if (socket == 'b')
	{
		terra_switch_transmit(CONF_SWITCH.code_boff);
	}
	else
	{
		terra_switch_transmit(CONF_SWITCH.code_coff);
	}

	terra_log_info("[terra_switch] set switch %c to off\n", socket);
}

static inline void terra_switch_set(char const socket, terra_switch_mode const mode)
{
	if (mode == SWITCH_ON)
	{
		terra_switch_on(socket);
	}
	else
	{
		terra_switch_off(socket);
	}
}

#endif
