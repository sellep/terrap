#ifndef __P_TERRA_VISUAL_H
#define __P_TERRA_VISUAL_H

#include "../terra_defs.h"
#include "../utils/terra_time.h"
#include "../utils/terra_data.h"

#define GRID_PADDING_TOP 5
#define GRID_PADDING_BOTTOM 2

#define GRID_OFFSET_TOP 2
#define GRID_OFFSET_BOTTOM 3
#define GRID_OFFSET_LEFT 5
#define GRID_OFFSET_RIGHT 2

#define DRAW_HEIGHT (height - GRID_OFFSET_TOP - GRID_OFFSET_BOTTOM)

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

	cmd[0] = atoi(argv[2]);
	terra_log_error("[terra_visual_arg] %zu\n", *cmd);
	return TRUE;
}

BOOL terra_visual_show(terra_visual_cmd const);

#endif