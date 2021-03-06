#ifndef __P_TERRA_VISUAL_H
#define __P_TERRA_VISUAL_H

#include "../terra_defs.h"
#include "../utils/terra_time.h"
#include "../utils/terra_data.h"

#ifdef NCURSES
#include <ncurses.h>
#endif

#define GRID_OFFSET_TOP 2
#define GRID_OFFSET_BOTTOM 1
#define GRID_OFFSET_LEFT 5
#define GRID_OFFSET_RIGHT 2

#define GRID_MARKER_Y 3
#define GRID_MARKER_X 10

#define DRAW_HEIGHT (height - GRID_OFFSET_TOP - GRID_OFFSET_BOTTOM)
#define DRAW_WIDTH (width - GRID_OFFSET_LEFT - GRID_OFFSET_RIGHT)

#define COLOR_DEFAULT COLOR_WHITE
#define COLOR_GRID COLOR_YELLOW
#define COLOR_LEGEND COLOR_WHITE
#define COLOR_TEMP COLOR_CYAN
#define COLOR_HUMI COLOR_MAGENTA
#define COLOR_BOTH COLOR_BLUE

#define SET_COLOR_DEFAULT() (attron(COLOR_PAIR(1)))
#define SET_COLOR_GRID() (attron(COLOR_PAIR(2)))
#define SET_COLOR_LEGEND() (attron(COLOR_PAIR(3)))
#define SET_COLOR_TEMP() (attron(COLOR_PAIR(4)))
#define SET_COLOR_HUMI() (attron(COLOR_PAIR(5)))
#define SET_COLOR_BOTH() (attron(COLOR_PAIR(6)))

typedef struct
{
	float *vals_y;
	size_t *vals_x;

	int *vals_humi;
	int *vals_temp;

	float min_humi;
	float max_humi;
	float min_temp;
	float max_temp;
} terra_visual_grid;

enum terra_visual_modes
{
	TERRA_BOTH,
	TERRA_TEMP,
	TERRA_HUMI
};

typedef int terra_visual_mode;

typedef struct
{
	ssize_t visual_argc;
	terra_time start;
	terra_time end;

	ssize_t days_past;
} terra_visual_cmd;

static inline BOOL terra_visual_arg(terra_visual_cmd * const cmd, ssize_t const argc, char * * const argv)
{
	cmd->visual_argc = argc - 2;
	cmd->days_past = 0;

	if (argc == 2)
		return TRUE;

	cmd->days_past = atoi(argv[2]);

	if (argc == 3)
		return TRUE;

	if (!terra_time_parse(&cmd->start, argv[3], HOUR_MIN))
	{
		terra_log_error("[terra_visual_arg] failed to parse start time (%s)\n", argv[3]);
		return FALSE;
	}

	if (argc == 4)
		return TRUE;

	if (!terra_time_parse(&cmd->end, argv[4], HOUR_MIN))
	{
		terra_log_error("[terra_visual_arg] failed to parse end time (%s)\n", argv[4]);
		return FALSE;
	}

	return TRUE;
}

extern BOOL terra_visual_show(terra_visual_cmd const * const);

#endif
