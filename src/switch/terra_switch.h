#ifndef __P_TERRA_SWITCH_H
#define __P_TERRA_SWITCH_H

#include "../terra_defs.h"
#include "../utils/terra_log.h"
#include "../utils/terra_lock.h"

#define SWITCH_FILE "terra_switch"

enum terra_switch_modes
{
	SWITCH_UNKNOWN = -1,
	SWITCH_OFF  = 0,
	SWITCH_ON = 1
};

typedef struct
{
	int channel;
	int code_aon;
	int code_aoff;
	int code_bon;
	int code_boff;
	int code_con;
	int code_coff;
} terra_switch_group;

typedef struct
{
	int pin;
	int repeats;

	int group_len;
	terra_switch_group *groups;
} terra_conf_switch;

typedef int terra_switch_mode;

typedef struct
{
	char socket;
	int channel;
	terra_switch_mode mode;
} terra_switch_req;

extern BOOL terra_switch_arg(terra_switch_req * const, int const, char * * const);
extern void terra_switch_transmit(terra_conf_switch const * const, int const);
extern BOOL terra_switch_init(terra_switch_mode * * const, terra_conf_switch const * const)

static inline terra_switch_group* terra_switch_group_get(terra_conf_switch const * const conf, int const channel)
{
	size_t i;

	if (channel == 0)
		return &conf->groups[0];

	for (i = 0; i < conf->group_len; i++)
	{
		if (conf->groups[i].channel == channel)
			return &conf->groups[i];
	}

	terra_log_error("[terra_switch_group_get] group with channel %i not found\n", channel);
	return NULL;
}

static inline void terra_switch_set_on(terra_conf_switch const * const conf, char const socket, int const channel, char const * const sender)
{
	terra_switch_group *group = terra_switch_group_get(conf, channel);

	if (socket == 'a')
	{
		terra_switch_transmit(conf, group->code_aon);
	}
	else if (socket == 'b')
	{
		terra_switch_transmit(conf, group->code_bon);
	}
	else
	{
		terra_switch_transmit(conf, group->code_con);
	}

	terra_log_info("[terra_switch] set switch %c:%i to on (%s)\n", socket, channel, sender);
}

static inline void terra_switch_set_off(terra_conf_switch const * const conf, char const socket, int const channel, char const * const sender)
{
	terra_switch_group *group = terra_switch_group_get(conf, channel);

	if (socket == 'a')
	{
		terra_switch_transmit(conf, group->code_aoff);
	}
	else if (socket == 'b')
	{
		terra_switch_transmit(conf, group->code_boff);
	}
	else
	{
		terra_switch_transmit(conf, group->code_coff);
	}

	terra_log_info("[terra_switch] set switch %c:%i to off (%s)\n", socket, channel, sender);
}

static inline void terra_switch_set(terra_conf_switch const * const conf, char const socket, int const channel, terra_switch_mode const mode, char const * const sender)
{
	if (mode == SWITCH_ON)
	{
		terra_switch_set_on(conf, socket, channel, sender);
	}
	else if (mode == SWITCH_OFF)
	{
		terra_switch_set_off(conf, socket, channel, sender);
	}
}

#endif
