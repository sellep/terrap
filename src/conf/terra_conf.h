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

enum terra_schedule_types
{
	SCHEDULE_CLOCK,
	SCHEDULE_PERIOD,
	SCHEDULE_HYGRO
};

typedef int terra_schedule_type;

typedef struct
{
	char *name;
	terra_schedule_type type;

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
} terra_conf_clock_mode;

typedef struct
{
	terra_conf_schedule schedule;

	terra_start_stop default_time;
	BOOL default_time_set;

	int mode_len;
	terra_conf_clock_mode *modes;

	terra_start_stop *time;
} terra_conf_schedule_clock;

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
} terra_conf_schedule_hygro;

typedef struct
{
	int delay;
	BOOL read_only;

	terra_conf_led ld;
	terra_conf_switch sw;
	terra_conf_hygro hy;

	int clock_len;
	terra_conf_schedule_clock *clocks;

	int hygro_len;
	terra_conf_schedule_hygro *hygros;
} terra_conf;

extern BOOL terra_conf_parse(terra_conf * const, char const * const);
extern void terra_conf_print(terra_conf const * const);
extern void terra_conf_free(terra_conf * const);

extern void terra_conf_schedule_print(terra_conf_schedule const * const);
extern BOOL terra_conf_schedule_parse(terra_conf_schedule * const, config_setting_t * const, terra_schedule_type const);
extern void terra_conf_schedule_free(terra_conf_schedule * const);

extern void terra_conf_schedule_clock_print(terra_conf_schedule_clock const * const);
extern BOOL terra_conf_schedule_clock_parse(terra_conf_schedule_clock * * const, int * const, config_t * const);
extern void terra_conf_schedule_clock_free(terra_conf_schedule_clock * const, int const);

extern void terra_conf_schedule_hygro_print(terra_conf_schedule_hygro const * const);
extern BOOL terra_conf_schedule_hygro_parse(terra_conf_schedule_hygro * * const, int * const, config_t * const);
extern void terra_conf_schedule_hygro_free(terra_conf_schedule_hygro * const, int const);

extern void config_string_copy(char * * const, char const * const);
extern BOOL config_parse_float(float * const, config_setting_t * const, char const * const);

#endif
