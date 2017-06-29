#include "terra_visual.h"

void terra_visual_points(ssize_t const width, ssize_t const height, terra_visual_point const * const pts)
{
#ifdef NCURSES
	wchar_t chr = L'\x2736';
	ssize_t x;

	SET_COLOR_TEMP();

	for (x = 1; x < DRAW_WIDTH; x++)
	{
		mvaddnwstr(height - GRID_OFFSET_BOTTOM - pts[x].temp, x + GRID_OFFSET_LEFT, &chr, 1);
	}

	SET_COLOR_HUMI();
	
	for (x = 1; x < DRAW_WIDTH; x++)
	{
		mvaddnwstr(height - GRID_OFFSET_BOTTOM - pts[x].humi, x + GRID_OFFSET_LEFT, &chr, 1);
	}
#endif
}