#include "terra_visual.h"

#define DETAILS_FORMAT "mode: %s, entries: %zu, temp: %.2f - %.2f, humi: %.2f - %.2f"

static inline void visual_draw_metadata(terra_visual_grid const * const grid, size_t const len, terra_visual_mode const mode)
{
	if (mode == TERRA_BOTH)
	{
		mvprintw(1, 0, "mode: BOTH, entries: %zu, humi: %.1f - %.1f, temp: %.1f - %.1f", len, grid->min_humi, grid->max_humi, grid->min_temp, grid->max_temp);
	}
	else if (mode == TERRA_HUMI)
	{
		mvprintw(1, 0, "mode: HUMI, entries: %zu, humi: %.1f - %.1f", len, grid->min_humi, grid->max_humi);
	}
	else
	{
		mvprintw(1, 0, "mode: TEMP, entries: %zu, temp: %.1f - %.1f", len, grid->min_temp, grid->max_temp);
	}
}

void terra_visual_draw_title(terra_visual_grid const * const grid, char const * const title, ssize_t const width, size_t const entries, terra_visual_mode const mode)
{
#ifdef NCURSES
	ssize_t tlen;
	ssize_t len;
	ssize_t i, j;

	tlen = strlen(title);
	len = (width - tlen) / 2;

	for (i = 0, j = tlen; i < len; i++, j++)
	{
		printw("-");
	}

	printw(title);

	for (i = 0; i < len; i++, j++)
	{
		printw("-");
	}

	if (j < width)
	{
		printw("-");
	}

	visual_draw_metadata(grid, entries, mode);
#endif
}
