#ifndef __P_TERRA_CONF_CLOCK_H
#define __P_TERRA_CONF_CLOCK_H

#include "tera_conf.h"

typedef struct
{
	char *name;
	terra_start_stop time;
} terra_clock_mode;

typedef struct
{
	terra_schedule schedule;

	terra_start_stop default_time;
	BOOL default_time_set;

	int mode_len;
	terra_clock_mode *modes;

	terra_start_stop *time;
} terra_conf_clock;

extern void terra_conf_clock_print(terra_conf_clock const * const);
extern BOOL terra_conf_clock_parse(terra_conf_clock * * const, int * const, config_t * const);
extern void terra_conf_clock_free(terra_conf_clock const * const, int const);

#endif