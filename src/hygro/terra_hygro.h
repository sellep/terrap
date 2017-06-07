#ifndef __P_TERRA_HYGRO_H
#define __P_TERRA_HYGRO_H

#include "../conf/terra_conf.h"

typedef struct
{
	float temp;
	float humi;
} hygro_res;

BOOL terra_hygro_read(hygro_res * const, terra_conf const * const);

#endif