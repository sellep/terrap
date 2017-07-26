#ifndef __P_TERRA_CONF_HYGRO_H
#define __P_TERRA_CONF_HYGRO_H

#include "tera_conf.h"

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
	terra_schedule schedule;

	terra_conf_hygro_set default_set;
	terra_conf_hygro_set *set;

	int mode_len;
	terra_hygro_mode *modes;
} terra_conf_hygro;

extern void terra_conf_hygro_print(terra_conf_hygro const * const);
extern BOOL terra_conf_hygro_parse(terra_conf_hygro * * const, int * const, config_t * const);
extern void terra_conf_hygro_free(terra_conf_hygro const * const, int const);

#endif