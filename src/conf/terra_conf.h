#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra_defs.h"

#include "../utils/terra_time.h"

#include <libconfig.h>

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

typedef struct
{
	char *name;
	char socket;
	BOOL enabled;

	char *depends;
	terra_switch_mode state;
} terra_scheduler;

typedef struct
{
	terra_scheduler scheduler;

	int time_len;
	terra_start_stop *times;
} terra_scheduler_clock;

typedef struct
{
	terra_scheduler scheduler;
	terra_time active;
	terra_time deactive;
	BOOL active_first;
} terra_scheduler_period;

typedef struct
{
	int delay;
	BOOL read_only;
	int pin_alert;

	terra_conf_heart he;
	terra_conf_switch sw;
	terra_conf_hygro hy;

	int clock_len;
	terra_scheduler_clock *clocks;

	int period_len;
	terra_scheduler_period *periods;
} terra_conf;

#define SCHEDULER_DISABLED(s) !(((terra_scheduler*)(s))->enabled)
#define SCHEDULER_SET_MODE(s, m) ((terra_scheduler*)(s))->state = m

extern BOOL terra_conf_read(terra_conf * const, char const * const);
extern void terra_conf_print(terra_conf const * const);

#endif
