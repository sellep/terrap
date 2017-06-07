#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra.h"
#include "../utils/terra_time.h"

#define HANDLE_ERROR(s){		\
	terra_log_error("%s", s);	\
	goto error;}

#define TERRA_CONF_MAX_SCHED_CLOCKS 10

enum sock_modes
{
	SOCK_UNKNOWN = 0,
	SOCK_ON = 1,
	SOCK_OFF = 2
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
	ssize_t tran_pin;
	ssize_t tran_rep;
	size_t sock_chan;
	size_t sock_code_a_on;
	size_t sock_code_a_off;
	size_t sock_code_b_on;
	size_t sock_code_b_off;
	size_t sock_code_c_on;
	size_t sock_code_c_off;

	ssize_t hygro_tick;
	char hygro_sensor[6];

	terra_sched_clock sched_clocks[TERRA_CONF_MAX_SCHED_CLOCKS];
	ssize_t sched_clocks_len;
} terra_conf;

void terra_conf_print(terra_conf const * const);
BOOL terra_conf_read(terra_conf * const, char const * const);
BOOL terra_setup(terra_conf const * const);

#endif
