#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "terra_log.h"

//gcc `ncursesw5-config --cflags` hello_nc.c `ncursesw5-config --libs`

#define GRID_OFFSET_TOP 2
#define GRID_OFFSET_BOTTOM 5
#define GRID_OFFSET_LEFT 5
#define GRID_OFFSET_RIGHT 2
#define GRID_MARKER_Y 3
#define GRID_MARKER_X 10

#define GRID_PADDING 5

#define DRAW_HEIGHT (height - GRID_OFFSET_TOP - GRID_OFFSET_BOTTOM)
#define DRAW_WIDTH (width - GRID_OFFSET_LEFT - GRID_OFFSET_RIGHT)

#define SET_COLOR_DEFAULT() (attron(COLOR_PAIR(1)))
#define SET_COLOR_GRID() (attron(COLOR_PAIR(2)))
#define SET_COLOR_LEGEND() (attron(COLOR_PAIR(3)))
#define SET_COLOR_TEMP() (attron(COLOR_PAIR(4)))
#define SET_COLOR_HUMI() (attron(COLOR_PAIR(5)))

typedef struct
{
	float ymin;
	float ymax;
	terra_time xmin;
	terra_time xmax;
} terra_draw_bounds;

typedef struct
{
	size_t temp;
	size_t humi;
} terra_draw_point;

inline static void itott(terra_time * const tt, size_t const val)
{
	size_t copy = val;

	tt->hour = copy / 60 / 60;
	copy -= tt->hour * 60 * 60;
	tt->min = copy / 60;
	copy -= tt->min * 60;
	tt->sec = copy;
}

inline static size_t tttoi(terra_time const * const tt)
{
	return tt->hour * 60 * 60 + tt->min * 60 + tt->sec;
}

inline static ssize_t tttoa(char * const buf, terra_time const * const tt)
{
	return sprintf(buf, "%zu:%02zu:%02zu", tt->hour, tt->min, tt->sec);
}

static void terra_draw_title(char const * const title, ssize_t const width)
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

inline static void terra_draw_grid_y(ssize_t width, ssize_t height)
{
	const wchar_t line = L'\x2502';
	const wchar_t marker = L'\x2524';
	const wchar_t cross = L'\x253C';

	ssize_t y, x, m;

	for (y = height - GRID_OFFSET_BOTTOM - 1, m = 1; y > GRID_OFFSET_TOP; y--, m++)
	{
		//use cross if show grid, otherwise marker
		mvaddnwstr(y, GRID_OFFSET_LEFT, m % GRID_MARKER_Y == 0 ? &cross : &line, 1);
	}

	//if show grid

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

inline static void terra_draw_grid_x(ssize_t const width, ssize_t const height)
{
	const wchar_t line = L'\x2500';
	const wchar_t marker = L'\x252C';
	const wchar_t cross = L'\x253C';

	ssize_t x, y, m;

	for (x = GRID_OFFSET_LEFT + 1, m = 1; x < width - GRID_OFFSET_RIGHT; x++, m++)
	{
		//use cross if show grid, otherwise marker
		mvaddnwstr(height - GRID_OFFSET_BOTTOM, x, m % GRID_MARKER_X == 0 ? &cross : &line, 1);
	}

	//if show grid

	SET_COLOR_GRID();

	for (y = GRID_MARKER_Y; y < DRAW_HEIGHT; y += GRID_MARKER_Y)
	{
		for (x = 1; x < DRAW_WIDTH; x++)
		{
			mvaddnwstr(height - GRID_OFFSET_BOTTOM - y, x + GRID_OFFSET_LEFT, x % GRID_MARKER_X == 0 ? &cross : &line, 1);
		}
	}

	SET_COLOR_DEFAULT();
}

static void terra_draw_grid(ssize_t const width, ssize_t const height)
{
	const wchar_t origin = L'\x253c';

	mvaddnwstr(height - GRID_OFFSET_BOTTOM, GRID_OFFSET_LEFT, &origin, 1);
	terra_draw_grid_y(width, height);
	terra_draw_grid_x(width, height);
}

static void terra_draw_labels(ssize_t const width, ssize_t const height, terra_draw_bounds const * const bounds)
{
	float y_val;
	float step;

	terra_time tt;
	size_t x_min;
	size_t x_max;
	size_t x_val;
	ssize_t mlen;
	char buf[8];

	ssize_t lbls;
	ssize_t i;

	//compute vertical (y) markers

	lbls = (height - GRID_OFFSET_BOTTOM - GRID_OFFSET_TOP) / GRID_MARKER_Y + 1;
	step = (bounds->ymax - bounds->ymin) / (lbls - 1);
	y_val = bounds->ymin;

	for (i = 0; i < lbls; i++)
	{
		mvprintw(height - GRID_OFFSET_BOTTOM - i * GRID_MARKER_Y, 1, "%.1f", y_val);
		y_val += step;
	}

	//compute horizontal (x) markers

	x_min = tttoi(&bounds->xmin);
	x_max = tttoi(&bounds->xmax);

	lbls = (width - GRID_OFFSET_LEFT - GRID_OFFSET_RIGHT) / GRID_MARKER_X + 1;
	step = (x_max - x_min) / (lbls - 1);
	x_val = x_min;

	for (i = 0; i < lbls; i++)
	{
		itott(&tt, x_val);
		mlen = tttoa(buf, &tt);
		mvprintw(height - GRID_OFFSET_BOTTOM + 1, GRID_OFFSET_LEFT + i * GRID_MARKER_X - (mlen / 2), "%s", buf);
		x_val += step;
	}
}

static void terra_draw_points(ssize_t const width, ssize_t const height, terra_draw_point const * const pts)
{
	wchar_t chr = L'\x2736';
	ssize_t x;

	SET_COLOR_TEMP();

	for (x = 1; x < DRAW_WIDTH; x++)
	{
		mvaddnwstr(height - GRID_OFFSET_BOTTOM - pts[x].temp, x + GRID_OFFSET_LEFT, &chr, 1);
	}
}

static void terra_draw_bounding(terra_draw_bounds * const bounds, terra_log_entry const * const entries, size_t const len)
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

	bounds->ymin -= GRID_PADDING;
	bounds->ymax += GRID_PADDING;
}


static void terra_draw_eval(ssize_t const width, ssize_t const height, terra_draw_bounds const * const bounds, terra_draw_point * const pts, terra_log_entry const * const entries, size_t const count)
{
	ssize_t x;
	float step = (float) count / (width - 1);

	for (x = 0; x < width -1; x++)
	{
		pts[x].temp = (size_t)((entries[(size_t)(x * step)].temp - bounds->ymin) / (bounds->ymax - bounds->ymin) * DRAW_HEIGHT);
	}
}

static void terra_draw_legend(ssize_t const width, ssize_t const height)
{
	const wchar_t corner_top_left = L'\x250f';
	const wchar_t corner_top_right = L'\x2513';
	const wchar_t corner_bottom_left = L'\x2517';
	const wchar_t corner_bottom_right = L'\x251b';
	const wchar_t line_vertical = L'\x2503';
	const wchar_t line_horizontal = L'\x2501';
	const wchar_t point = L'\x2736';

	SET_COLOR_LEGEND();

	mvaddnwstr(GRID_OFFSET_TOP, width - GRID_OFFSET_RIGHT - 7, &corner_top_left, 1);
	mvaddnwstr(GRID_OFFSET_TOP, width - GRID_OFFSET_RIGHT - 7 + 6, &corner_top_right, 1);
	mvaddnwstr(GRID_OFFSET_TOP + 3, width - GRID_OFFSET_RIGHT - 7, &corner_bottom_left, 1);
	mvaddnwstr(GRID_OFFSET_TOP + 3, width - GRID_OFFSET_RIGHT - 7 + 6, &corner_bottom_right, 1);

	mvaddnwstr(GRID_OFFSET_TOP, width - GRID_OFFSET_RIGHT - 7 + 1, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP, width - GRID_OFFSET_RIGHT - 7 + 2, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP, width - GRID_OFFSET_RIGHT - 7 + 3, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP, width - GRID_OFFSET_RIGHT - 7 + 4, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP, width - GRID_OFFSET_RIGHT - 7 + 5, &line_horizontal, 1);

	mvaddnwstr(GRID_OFFSET_TOP + 1, width - GRID_OFFSET_RIGHT - 7, &line_vertical, 1);
	mvaddnwstr(GRID_OFFSET_TOP + 2, width - GRID_OFFSET_RIGHT - 7, &line_vertical, 1);

	mvaddnwstr(GRID_OFFSET_TOP + 3, width - GRID_OFFSET_RIGHT - 7 + 1, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP + 3, width - GRID_OFFSET_RIGHT - 7 + 2, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP + 3, width - GRID_OFFSET_RIGHT - 7 + 3, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP + 3, width - GRID_OFFSET_RIGHT - 7 + 4, &line_horizontal, 1);
	mvaddnwstr(GRID_OFFSET_TOP + 3, width - GRID_OFFSET_RIGHT - 7 + 5, &line_horizontal, 1);

	mvaddnwstr(GRID_OFFSET_TOP + 1, width - GRID_OFFSET_RIGHT - 7 + 6, &line_vertical, 1);
	mvaddnwstr(GRID_OFFSET_TOP + 2, width - GRID_OFFSET_RIGHT - 7 + 6, &line_vertical, 1);

	SET_COLOR_TEMP();

	mvaddnwstr(GRID_OFFSET_TOP + 1, width - GRID_OFFSET_RIGHT - 7 + 1, &point, 1);

	SET_COLOR_HUMI();

	mvaddnwstr(GRID_OFFSET_TOP + 2, width - GRID_OFFSET_RIGHT - 7 + 1, &point, 1);

	SET_COLOR_DEFAULT();

	mvprintw(GRID_OFFSET_TOP + 1, width - GRID_OFFSET_RIGHT - 7 + 2, "temp");
	mvprintw(GRID_OFFSET_TOP + 2, width - GRID_OFFSET_RIGHT - 7 + 2, "humi");
}

void terra_draw(char const * const path, terra_log_entry const * const entries, size_t const count)
{
	terra_draw_bounds bounds;
	terra_draw_point *pts;
	int width;
	int height;

	terra_draw_bounding(&bounds, entries, count);

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

	pts = (terra_draw_point*) malloc(sizeof(terra_draw_point) * (width - 1));
	terra_draw_eval(width, height, &bounds, pts, entries, count);

	terra_draw_title(path, width);
	terra_draw_grid(width, height);
	terra_draw_labels(width, height, &bounds);
	terra_draw_legend(width, height);
	terra_draw_points(width, height, pts);

	getch();

	endwin();
}

#define PATH "test_data"

int main(int argc, char * * argv)
{
	terra_log_entry *entries;
	int lines;

	lines = terra_log_read(&entries, PATH);
	if (lines <= 0)
	{
		fprintf(stderr, "failed to read file\n");
	}

	terra_draw(PATH, entries, lines);

	free(entries);
	return 0;
}
