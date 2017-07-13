#include "terra_visual.h"

static inline ssize_t labels_remove_secs(char * const lbl)
{
	size_t i = 0;
	size_t c = 0;

	while (TRUE)
	{
		if (lbl[i++] != ':')
			continue;

		if (c++ == 0)
			continue;

		lbl[i - 1] = '\0';
		return i - 1;
	}
}

static inline void draw_labels_y(terra_visual_grid const * const grid, ssize_t const height)
{
#ifdef NCURSES
	ssize_t y;

	for (y = 0; y < DRAW_HEIGHT; y++)
	{
		if (y % GRID_MARKER_Y == 0)
		{
			mvprintw(GRID_OFFSET_TOP + DRAW_HEIGHT - (y + 1), 0, "%.1f", grid->vals_y[y]);
		}
	}
#endif
}

static inline void draw_labels_x(terra_visual_grid const * const grid, ssize_t const width, ssize_t const height)
{
#ifdef NCURSES
	char buf[10];
	terra_time tm;
	ssize_t len;
	ssize_t x;

	for (x = 0; x < DRAW_WIDTH; x++)
	{
		if (x % GRID_MARKER_X == 0)
		{
			terra_time_from_int(&tm, grid->vals_x[x]);
			terra_time_to_arr(buf, &tm);
			len = labels_remove_secs(buf);

			mvprintw(GRID_OFFSET_TOP + DRAW_HEIGHT, GRID_OFFSET_LEFT + x - (len / 2), "%s", buf);
		}
	}
#endif
}

void terra_visual_draw_labels(ssize_t const width, ssize_t const height, terra_visual_grid const * const grid)
{

	draw_labels_y(grid, height);
	draw_labels_x(grid, width, height);
}
