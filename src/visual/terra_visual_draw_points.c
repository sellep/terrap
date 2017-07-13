#include "terra_visual.h"

void terra_visual_draw_points(ssize_t const width, ssize_t const height, terra_visual_point const * const pts)
{
#ifdef NCURSES
	wchar_t chr = L'\x2736';
	ssize_t x;

	SET_COLOR_TEMP();

	for (x = 1; x < DRAW_WIDTH; x++)
	{
		if (pts[x].temp == pts[x].humi)
			continue;

		mvaddnwstr(height - GRID_OFFSET_BOTTOM - 1 - pts[x].temp, x + GRID_OFFSET_LEFT, &chr, 1);
	}

	SET_COLOR_HUMI();
	
	for (x = 1; x < DRAW_WIDTH; x++)
	{
		if (pts[x].humi == pts[x].temp)
		{
			SET_COLOR_BOTH();
			mvaddnwstr(height - GRID_OFFSET_BOTTOM - 1 - pts[x].humi, x + GRID_OFFSET_LEFT, &chr, 1);
			SET_COLOR_HUMI();
		}
		else
		{
			mvaddnwstr(height - GRID_OFFSET_BOTTOM - 1 - pts[x].humi, x + GRID_OFFSET_LEFT, &chr, 1);
		}
	}
#endif
}
