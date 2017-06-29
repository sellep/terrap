#include "terra_visual.h"

#include "terra_data.h"

#include <unistd.h>
#include <locale.h>
#include <wchar.h>
#include <stddef.h>

#ifdef NCURSES
#include <ncursesw/ncurses.h>

#define GRID_PADDING_TOP 5
#define GRID_PADDING_BOTTOM 2

#define GRID_OFFSET_TOP 2
#define GRID_OFFSET_BOTTOM 3
#define GRID_OFFSET_LEFT 5
#define GRID_OFFSET_RIGHT 2

#define DRAW_HEIGHT (height - GRID_OFFSET_TOP - GRID_OFFSET_BOTTOM)

inline static void terra_visual_title(char const * const title, ssize_t const width)
{
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
}

inline static void terra_visual_eval(ssize_t const width, ssize_t const height, terra_visual_bounds const * const bounds, terra_visual_point * const pts, terra_data_entry const * const entries, size_t const count)
{
	ssize_t x;
	float step = (float) count / (width - 1);

	for (x = 0; x < width -1; x++)
	{
		pts[x].temp = (size_t)((entries[(size_t)(x * step)].temp - bounds->ymin) / (bounds->ymax - bounds->ymin) * DRAW_HEIGHT);
	}
}

inline static void terra_visual_bounding(terra_visual_bounds * const bounds, terra_data_entry const * const entries, size_t const len)
{
	size_t i;

	bounds->ymin = entries[0].humi < entries[0].temp ? entries[0].humi : entries[0].temp;
	bounds->ymax = entries[0].humi > entries[0].temp ? entries[0].humi : entries[0].temp;
	bounds->xmin = entries[0].tm;
	bounds->xmax = entries[len - 1].tm;

	for (i = 1; i < len; i++)
	{
		if (entries[i].humi > bounds->ymax)
		{
			bounds->ymax = entries[i].humi;
		}
		else if (entries[i].humi < bounds->ymin)
		{
			bounds->ymin = entries[i].humi;
		}

		if (entries[i].temp > bounds->ymax)
		{
			bounds->ymax = entries[i].temp;
		}
		else if (entries[i].temp < bounds->ymin)
		{
			bounds->ymin = entries[i].temp;
		}
	}

	bounds->ymin -= GRID_PADDING_BOTTOM;
	bounds->ymax += GRID_PADDING_TOP;
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
