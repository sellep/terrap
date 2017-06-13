#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra_defs.h"
#include "../utils/terra_log.h"
#include "../utils/terra_time.h"

#define HANDLE_ERROR(s){ \
	terra_log_error("%s", s); \
	goto error;}

#define TERRA_CONF_MAX_SCHED_TEMPS 5
#define TERRA_CONF_MAX_SCHED_PERIODS 5
#define TERRA_CONF_MAX_SCHED_CLOCKS 5
#define TERRA_CONF_MAX_SCHED_CLOCK_TIMES 5

#define TRY_CONTINUE(l) \
	if (l[0] == '\0') continue; \
	if (l[0] == '\n') continue; \
	if (l[0] == '#') continue;

#define SCHED_DISABLED(s)(((terra_sched*) s)->enabled == 0)

enum switch_modes
{
	SWITCH_UNKNOWN = 0,
	SWITCH_ON = 1,
	SWITCH_OFF = 2
};

typedef ssize_t switch_mode;

enum sched_triggers
{
	TRIGGER_CLOCK = 1,
	TRIGGER_TEMP = 2,
	TRIGGER_MOIS = 3,
	TRIGGER_PERIOD = 4
};

typedef int sched_trig;

typedef struct
{
	char name[30];
	sched_trig trig;
	char sock;
	ssize_t enabled;
} terra_sched;

typedef struct
{
	terra_sched sched;
	terra_start_stop times[TERRA_CONF_MAX_SCHED_CLOCK_TIMES];
	ssize_t times_len;
} terra_sched_clock;

typedef struct
{
	terra_sched sched;
	terra_time on_dur;
	terra_time off_dur;
	terra_time act;
	terra_time deact;
} terra_sched_period;

typedef struct
{
	terra_sched sched;
	float act_lim;
	float deact_lim;
	ssize_t invert;
} terra_sched_temp;

typedef struct
{
	ssize_t tick;
	ssize_t led_pin_heart;
	ssize_t heart_tick;
	ssize_t heart_dur;
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
	ssize_t hygro_pin_io;
	ssize_t hygro_tick;
	ssize_t hygro_rep;
	ssize_t hygro_write_secs;

	terra_sched_clock sched_clocks[TERRA_CONF_MAX_SCHED_CLOCKS];
	ssize_t sched_clocks_len;

	terra_sched_temp sched_temps[TERRA_CONF_MAX_SCHED_TEMPS];
	ssize_t sched_temps_len;

	terra_sched_period sched_periods[TERRA_CONF_MAX_SCHED_PERIODS];
	ssize_t sched_periods_len;
} terra_conf;

void terra_conf_print(terra_conf const * const);
BOOL terra_conf_read(terra_conf * const, char const * const);
BOOL terra_setup(terra_conf const * const);
BOOL terra_conf_valid(terra_conf * const);

#endif
