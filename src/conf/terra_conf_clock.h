#ifndef __P_TERRA_CONF_CLOCK_H
#define __P_TERRA_CONF_CLOCK_H

#include "tera_conf.h"

extern void terra_conf_clock_print(terra_conf_clock const * const);
extern BOOL terra_conf_clock_parse(terra_conf_clock * * const, int * const, config_t * const);
extern void terra_conf_clock_free(terra_conf_clock const * const, int const);

#endif