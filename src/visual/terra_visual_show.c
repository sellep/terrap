#include "terra_visual.h"

#include <unistd.h>
#include <locale.h>
#include <wchar.h>
#include <stddef.h>

#ifdef NCURSES
#include <ncursesw/ncurses.h>

extern void terra_visual_title(char const * const, ssize_t const);
extern void terra_visual_bounding(terra_visual_bounds * const, terra_data_entry const * const, size_t const);

inline static void terra_visual_eval(ssize_t const width, ssize_t const height, terra_visual_bounds const * const bounds, terra_visual_point * const pts, terra_data_entry const * const entries, size_t const count)
{
	ssize_t x;
	float step = (float) count / (width - 1);

	for (x = 0; x < width -1; x++)
	{
		pts[x].temp = (size_t)((entries[(size_t)(x * step)].temp - bounds->ymin) / (bounds->ymax - bounds->ymin) * DRAW_HEIGHT);
	}
}

void terra_show(char const * const title, terra_data_entry const * const entries, size_t const count)
{
	terra_visual_bounds bounds;
	terra_visual_point *pts;
	int width;
	int height;

	terra_visual_bounding(&bounds, entries, count);

	setlocale(LC_CTYPE, "en_US.UTF-8");

	initscr();
	noecho();
	curs_set(FALSE);

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);

	getmaxyx(stdscr, height, width);

	pts = (terra_visual_point*) malloc(sizeof(terra_visual_point) * (width - 1));
	terra_visual_eval(width, height, &bounds, pts, entries, count);

	terra_visual_title(title, width);
	//terra_draw_grid(width, height);
	//terra_draw_labels(width, height, &bounds);
	//terra_draw_legend(width, height);
	//terra_draw_points(width, height, pts);

	getch();
	endwin();
}

#endif

BOOL terra_visual_show(terra_visual_cmd const cmd)
{
	terra_data_entry *entries;
	size_t count;
	char path[50];
terra_log_error("[terra_visual_show] cmd %zu\n", cmd);
	if (!terra_data_read(&entries, &count, path, cmd))
	{
		terra_log_error("[terra_visual_show] failed to read entries\n");
		return FALSE;
	}

#ifdef NCURSES
	terra_show(path, entries, count);
#else
	terra_log_info("[terra_visual_show] ncurses disabled\n");
#endif

	free(entries);
	return TRUE;
}
