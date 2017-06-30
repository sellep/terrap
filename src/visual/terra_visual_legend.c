#include "terra_visual.h"

void terra_visual_legend(ssize_t const width, ssize_t const height)
{
#ifdef NCURSES
	const wchar_t corner_top_left = L'\x250f';
	const wchar_t corner_top_right = L'\x2513';
	const wchar_t corner_bottom_left = L'\x2517';
	const wchar_t corner_bottom_right = L'\x251b';
	const wchar_t line_vertical = L'\x2503';
	const wchar_t line_horizontal = L'\x2501';
	const wchar_t point = L'\x2736';

	SET_COLOR_LEGEND();

	mvaddnwstr(GRID_OFFSET_TOP - 1, width - GRID_OFFSET_RIGHT - 7, &corner_top_left, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1, width - GRID_OFFSET_RIGHT - 7 + 6, &corner_top_right, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1 + 3, width - GRID_OFFSET_RIGHT - 7, &corner_bottom_left, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1 + 3, width - GRID_OFFSET_RIGHT - 7 + 6, &corner_bottom_right, 1);

	mvaddnwstr(GRID_OFFSET_TOP - 1, width - GRID_OFFSET_RIGHT - 7 + 1, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1, width - GRID_OFFSET_RIGHT - 7 + 2, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1, width - GRID_OFFSET_RIGHT - 7 + 3, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1, width - GRID_OFFSET_RIGHT - 7 + 4, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1, width - GRID_OFFSET_RIGHT - 7 + 5, &line_horizontal, 1);

	mvaddnwstr(GRID_OFFSET_TOP - 1 + 1, width - GRID_OFFSET_RIGHT - 7, &line_vertical, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1 + 2, width - GRID_OFFSET_RIGHT - 7, &line_vertical, 1);

	mvaddnwstr(GRID_OFFSET_TOP - 1 + 3, width - GRID_OFFSET_RIGHT - 7 + 1, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1 + 3, width - GRID_OFFSET_RIGHT - 7 + 2, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1 + 3, width - GRID_OFFSET_RIGHT - 7 + 3, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1 + 3, width - GRID_OFFSET_RIGHT - 7 + 4, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1 + 3, width - GRID_OFFSET_RIGHT - 7 + 5, &line_horizontal, 1);

	mvaddnwstr(GRID_OFFSET_TOP - 1 + 1, width - GRID_OFFSET_RIGHT - 7 + 6, &line_vertical, 1);
	mvaddnwstr(GRID_OFFSET_TOP - 1 + 2, width - GRID_OFFSET_RIGHT - 7 + 6, &line_vertical, 1);

	SET_COLOR_TEMP();

	mvaddnwstr(GRID_OFFSET_TOP - 1 + 1, width - GRID_OFFSET_RIGHT - 7 + 1, &point, 1);

	SET_COLOR_HUMI();

	mvaddnwstr(GRID_OFFSET_TOP - 1 + 2, width - GRID_OFFSET_RIGHT - 7 + 1, &point, 1);

	SET_COLOR_DEFAULT();

	mvprintw(GRID_OFFSET_TOP - 1 + 1, width - GRID_OFFSET_RIGHT - 7 + 2, "temp");
	mvprintw(GRID_OFFSET_TOP - 1 + 2, width - GRID_OFFSET_RIGHT - 7 + 2, "humi");
#endif
}