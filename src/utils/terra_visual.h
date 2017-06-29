#ifndef __P_TERRA_VISUAL_H
#define __P_TERRA_VISUAL_H

#include "../terra_defs.h"
#include "terra_time.h"

typedef struct
{
	float ymin;
	float ymax;
	terra_time xmin;
	terra_time xmax;
} terra_visual_bounds;

typedef struct
{
	size_t temp;
	size_t humi;
} terra_visual_point;

typedef ssize_t terra_visual_cmd;

inline static BOOL terra_visual_arg(terra_visual_cmd * const cmd, ssize_t const argc, char const * const * const argv)
{
	if (argc != 3)
	{
		terra_log_error("[terra_visual_arg] insufficient args");
		return FALSE;
	}

	cmd = atoi(argv[2]);
	return TRUE;
}

BOOL terra_visual_show(terra_visual_cmd const);

#endif