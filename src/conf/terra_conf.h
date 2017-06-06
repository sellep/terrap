#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra.h"

typedef struct
{
	ssize_t hour;
	ssize_t min;
	ssize_t sec;
} terra_time;

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
} terra_sched;

typedef struct
{
	terra_sched sched;
	terra_time start;
	terra_time end;
} terra_sched_clock;

typedef struct
{
	ssize_t tran_pin;
	ssize_t tran_rep;
	size_t sock_chan;
	size_t sock_code_a_on;
	size_t sock_code_a_off;
	size_t sock_code_b_on;
	size_t sock_code_b_off;
	size_t sock_code_c_on;
	size_t sock_code_c_off;

	terra_sched_clock sched_clocks[10];
	ssize_t sched_clocks_len;
} terra_conf;

void terra_conf_print(terra_conf const * const);
BOOL terra_conf_read(terra_conf * const, char const * const);
BOOL terra_setup(terra_conf const * const);

#endif
