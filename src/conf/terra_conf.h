#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra_defs.h"
#include "../utils/terra_time.h"

#include <libconfig.h>

#define CONF_PATH "/etc/terra/terra.conf"

typedef struct
{
	int pin;
	int tick;
	int duration;
} terra_conf_heart;

typedef struct
{
	int pin;
	int repeats;
	int channel;
	int code_aon;
	int code_aoff;
	int code_bon;
	int code_boff;
	int code_con;
	int code_coff;
} terra_conf_switch;

typedef struct
{
	BOOL enabled;
	int pin;
	int repeats;
	int delay;
} terra_conf_hygro;

enum schedule_types
{
	SCHEDULE_CLOCK,
	SCHEDULE_PERIOD,
	SCHEDULE_TEMP,
	SCHEDULE_HUMI
};

typedef int schedule_type;

typedef struct
{
	char *name;
	schedule_type type;
	char socket;
	BOOL enabled;
	char *dep;

	BOOL use_begin;
	size_t begin;
	size_t end;

	BOOL run;
} terra_schedule;

typedef struct
{
	terra_schedule schedule;

	int time_len;
	terra_start_stop *times;
} terra_schedule_clock;

typedef struct
{
	terra_schedule schedule;

	float act;
	float deact;
} terra_schedule_temp;

typedef struct
{
	terra_schedule schedule;

	terra_time act;
	terra_time deact;

	BOOL active_first;
} terra_schedule_period;

typedef struct
{
	int delay;
	BOOL read_only;
	int pin_alert;

	terra_conf_heart he;
	terra_conf_switch sw;
	terra_conf_hygro hy;

	int clock_len;
	terra_schedule_clock *clocks;

	int temp_len;
	terra_schedule_temp *temps;
} terra_conf;

extern BOOL terra_conf_read(terra_conf * const, char const * const);
extern void terra_conf_print(terra_conf const * const);

#endif
