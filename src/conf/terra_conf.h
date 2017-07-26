#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra_defs.h"
#include "../utils/terra_time.h"
#include "../utils/terra_mode.h"
#include "../led/terra_led.h"
#include "../switch/terra_switch.h"
#include "../hygro/terra_hygro.h"

#include <libconfig.h>

#define CONF_PATH "/etc/terra/terra.conf"

enum schedule_types
{
	SCHEDULE_CLOCK,
	SCHEDULE_PERIOD,
	SCHEDULE_HYGRO
};

typedef int schedule_type;

typedef struct
{
	char *name;
	schedule_type type;

	char socket;
	int channel;

	BOOL enabled;
	char *dep;

	BOOL run;
} terra_conf_schedule;

typedef struct
{
	char *name;
	terra_start_stop time;
} terra_clock_mode;

typedef struct
{
	terra_conf_schedule schedule;

	terra_start_stop default_time;
	BOOL default_time_set;

	int mode_len;
	terra_clock_mode *modes;

	terra_start_stop *time;
} terra_conf_clock;

enum terra_hygro_targets
{
	HYGRO_NONE = 0,
	HYGRO_TEMP = 1,
	HYGRO_HUMI = 2
};

typedef int terra_hygro_target;

#define TERRA_HYGRO_BOTH(t) (t) & HYGRO_TEMP && (t) & HYGRO_HUMI

typedef struct
{
	float humi_act;
	float humi_dact;
	float temp_act;
	float temp_deact;
	terra_hygro_target target;
} terra_conf_hygro_set;

typedef struct
{
	char *name;
	terra_conf_hygro_set set;
} terra_hygro_mode;

typedef struct
{
	terra_conf_schedule schedule;

	terra_conf_hygro_set default_set;
	terra_conf_hygro_set *set;

	int mode_len;
	terra_hygro_mode *modes;
} terra_conf_hygro;

typedef struct
{
	int delay;
	BOOL read_only;

	terra_conf_led ld;
	terra_conf_switch sw;
	terra_conf_hygro hy;

	int clock_len;
	terra_schedule_clock *clocks;

	int temp_len;
	terra_schedule_temp *temps;
} terra_conf;

extern BOOL terra_conf_read(terra_conf * const, char const * const);
extern void terra_conf_print(terra_conf const * const);
extern BOOL terra_conf_valid(terra_conf * const);


static inline void terra_conf_free(terra_conf * const conf)
{
	size_t i;

	free(conf->sw.groups);

	for (i = 0; i < conf->clock_len; i++)
	{
		terra_schedule_free(&conf->clocks[i].schedule);

		if (conf->clocks[i].mode_len > 0)
		{
			free(conf->clocks[i].modes);
		}
	}

	for (i = 0; i < conf->temp_len; i++)
	{
		terra_schedule_free(&conf->temps[i].schedule);
	}

	for (i = 0; i < conf->period_len; i++)
	{
		terra_schedule_free(&conf->periods[i].schedule);
	}

	if (conf->clock_len > 0)
	{
		free(conf->clocks);
	}

	if (conf->temp_len > 0)
	{
		free(conf->temps);
	}

	if (conf->period_len > 0)
	{
		free(conf->periods);
	}
}

#endif
