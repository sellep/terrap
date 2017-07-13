#include "terra_visual.h"

#ifdef NCURSES

static inline void visual_grid_y(ssize_t width, ssize_t height)
{
	const wchar_t line = L'\x2502';
	const wchar_t marker = L'\x2524';
	const wchar_t cross = L'\x253C';

	ssize_t y, x, m;

	for (y = height - GRID_OFFSET_BOTTOM - 2, m = 1; y >= GRID_OFFSET_TOP; y--, m++)
	{
		mvaddnwstr(y, GRID_OFFSET_LEFT, m % GRID_MARKER_Y == 0 ? &cross : &line, 1);
	}

	SET_COLOR_GRID();

	for (x = GRID_MARKER_X; x < DRAW_WIDTH; x += GRID_MARKER_X)
	{
		for (y = height - GRID_OFFSET_BOTTOM - 1; y > GRID_OFFSET_TOP; y--)
		{
			mvaddnwstr(y, GRID_OFFSET_LEFT + x, &line, 1);
		}
	}

	SET_COLOR_DEFAULT();
}

static inline void visual_grid_x(ssize_t const width, ssize_t const height)
{
	const wchar_t line = L'\x2500';
	const wchar_t marker = L'\x252C';
	const wchar_t cross = L'\x253C';

	ssize_t x, y, m;

	for (x = GRID_OFFSET_LEFT + 1, m = 1; x < width - GRID_OFFSET_RIGHT; x++, m++)
	{
		mvaddnwstr(height - GRID_OFFSET_BOTTOM - 1, x, m % GRID_MARKER_X == 0 ? &cross : &line, 1);
	}

	SET_COLOR_GRID();

	for (y = GRID_MARKER_Y; y < DRAW_HEIGHT; y += GRID_MARKER_Y)
	{
		for (x = 1; x < DRAW_WIDTH; x++)
		{
			mvaddnwstr(height - GRID_OFFSET_BOTTOM - y - 1, x + GRID_OFFSET_LEFT, x % GRID_MARKER_X == 0 ? &cross : &line, 1);
		}
	}

	SET_COLOR_DEFAULT();
}

static inline void draw_points(terra_visual_grid const * const grid, ssize_t const width, ssize_t const height)
{
	wchar_t chr = L'\x2736';
	ssize_t x;

	SET_COLOR_HUMI();

	for (x = 0; x < DRAW_WIDTH; x++)
	{
		mvaddnwstr(GRID_OFFSET_TOP + DRAW_HEIGHT - grid->vals_humi[x], GRID_OFFSET_LEFT + x, &chr, 1);
	}

	SET_COLOR_DEFAULT();
}

#endif

void terra_visual_draw_grid(terra_visual_grid const * const grid, ssize_t const width, ssize_t const height)
{
#ifdef NCURSES
	const wchar_t origin = L'\x253c';

	mvaddnwstr(height - GRID_OFFSET_BOTTOM - 1, GRID_OFFSET_LEFT, &origin, 1);
	visual_grid_y(width, height);
	visual_grid_x(width, height);
	draw_points(grid, width, height);
#endif
}

