#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra_defs.h"
#include "../utils/terra_time.h"
#include "../utils/terra_mode.h"
#include "../utils/terra_signal.h"
#include "../led/terra_led.h"
#include "../switch/terra_switch.h"
#include "../hygro/terra_hygro.h"

#include <libconfig.h>

#define CONF_PATH "/etc/terra/terra.conf"

enum terra_config_results
{
	CONFIG_PARSE_FAILED,
	CONFIG_PARSE_UNSET,
	CONFIG_PARSE_OK
};

typedef int terra_parse_result;

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

/** clock schedule section */

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

/** end */

/** hygro schedule section */

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
	float humi_deact;
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

	int mode_len;
	terra_hygro_mode *modes;

	terra_conf_hygro_set *set;
} terra_conf_schedule_hygro;

/** end */

/** period schedule section */

typedef struct
{
	size_t on_time;
	size_t off_time;
	BOOL off_start;
} terra_conf_period_set;

typedef struct
{
	char *name;
	terra_conf_period_set set;
} terra_period_mode;

typedef struct
{
	terra_conf_schedule schedule;

	terra_conf_period_set default_set;
	BOOL has_default_set;

	int mode_len;
	terra_period_mode *modes;

	terra_conf_period_set *set;
	terra_switch_mode state;
	terra_time start;
} terra_conf_schedule_period;

/** end */

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

	int period_len;
	terra_conf_schedule_period *periods;
} terra_conf;

extern terra_parse_result terra_conf_parse(terra_conf * const, char const * const);
extern BOOL terra_conf_run(int const, char * * const, terra_conf const * const);
extern void terra_conf_free(terra_conf * const);

extern void terra_conf_print_schedule(terra_conf_schedule const * const);
extern terra_parse_result terra_conf_parse_schedule(terra_conf_schedule * const, config_setting_t * const, terra_schedule_type const);
extern void terra_conf_free_schedule(terra_conf_schedule * const);

extern void terra_conf_print_schedule_clock(terra_conf_schedule_clock const * const);
extern terra_parse_result terra_conf_parse_schedule_clock(terra_conf_schedule_clock * * const, int * const, config_t * const);
extern void terra_conf_free_schedule_clock(terra_conf_schedule_clock * const, int const);

extern void terra_conf_print_schedule_hygro(terra_conf_schedule_hygro const * const);
extern terra_parse_result terra_conf_parse_schedule_hygro(terra_conf_schedule_hygro * * const, int * const, config_t * const);
extern void terra_conf_free_schedule_hygro(terra_conf_schedule_hygro * const, int const);

extern void terra_conf_print_schedule_period(terra_conf_schedule_period const * const);
extern terra_parse_result terra_conf_parse_schedule_period(terra_conf_schedule_period * * const, int * const, config_t * const);
extern void terra_conf_free_schedule_period(terra_conf_schedule_period * const, int const);

extern terra_parse_result config_parse_string(char * * const, config_setting_t * const, char const * const);
extern terra_parse_result config_parse_float(float * const, config_setting_t * const, char const * const);
extern terra_parse_result config_parse_time(terra_time * const, config_setting_t * const, char const * const);

#endif
