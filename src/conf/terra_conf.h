#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra.h"
#include "../utils/terra_time.h"

#define HANDLE_ERROR(s){		\
	terra_log_error("%s", s);	\
	goto error;}

#define TERRA_CONF_MAX_SCHED_CLOCKS 10

enum switch_modes
{
	SWITCH_UNKNOWN = 0,
	SWITCH_ON = 1,
	SWITCH_OFF = 2
};

enum sched_triggers
{
	TRIGGER_CLOCK = 1,
	TRIGGER_TEMP = 2,
	TRIGGER_MOIS = 3
};

typedef int sched_trig;

typedef struct
{
	char name[20];
	sched_trig trig;
	char sock;
	ssize_t enabled;
} terra_sched;

typedef struct
{
	terra_sched sched;
	terra_time start;
	terra_time end;
} terra_sched_clock;

typedef struct
{
	ssize_t tick;
	ssize_t led_pin;
	ssize_t switch_pin;
	ssize_t switch_rep;
	size_t switch_chan;
	size_t switch_code_aon;
	size_t switch_code_aoff;
	size_t switch_code_bon;
	size_t switch_code_boff;
	size_t switch_code_con;
	size_t switch_code_coff;

	ssize_t hygro_enabled;
	ssize_t hygro_pin;
	ssize_t hygro_tick;
	char hygro_sensor[6];

	terra_sched_clock sched_clocks[TERRA_CONF_MAX_SCHED_CLOCKS];
	ssize_t sched_clocks_len;
} terra_conf;

void terra_conf_print(terra_conf const * const);
BOOL terra_conf_read(terra_conf * const, char const * const);
BOOL terra_setup(terra_conf const * const);

#endif
