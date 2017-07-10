#ifndef __P_TERRA_CONF_H
#define __P_TERRA_CONF_H

#include "../terra_defs.h"

#include <libconfig.h>

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
	int delay;
	BOOL read_only;

	terra_conf_switch sw;
	terra_conf_hygro hy;
} terra_conf;

extern BOOL terra_conf_read(terra_conf * const, char const * const);

#endif