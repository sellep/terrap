#ifndef __P_TERRA_CONF_HYGRO_H
#define __P_TERRA_CONF_HYGRO_H

#include "tera_conf.h"

extern void terra_conf_hygro_print(terra_conf_hygro const * const);
extern BOOL terra_conf_hygro_parse(terra_conf_hygro * * const, int * const, config_t * const);
extern void terra_conf_hygro_free(terra_conf_hygro const * const, int const);

#endif