#include "terra_visual.h"

#include <unistd.h>
#include <locale.h>
#include <wchar.h>
#include <stddef.h>

#define KEY_ESC 27
#define KEY_F5 269

extern void terra_visual_grid_init(terra_visual_grid * const, terra_visual_cmd const * const, size_t const, size_t const, terra_data_entry const * const, size_t const, terra_visual_mode const);
extern void terra_visual_draw_title(terra_visual_grid const * const, char const * const, ssize_t const, size_t const, terra_visual_mode const);
extern void terra_visual_draw_grid(terra_visual_grid const * const, ssize_t const, ssize_t const, terra_visual_mode const);
extern void terra_visual_draw_labels(ssize_t const, ssize_t const, terra_visual_grid const * const);

static inline void show_shift_mode(terra_visual_mode * const mode)
{
	if (mode[0] == TERRA_BOTH)
	{
		mode[0] = TERRA_HUMI;
	}
	else if (mode[0] == TERRA_HUMI)
	{
		mode[0] = TERRA_TEMP;
	}
	else
	{
		mode[0] = TERRA_BOTH;
	}
}

static inline void terra_visual_grid_free(terra_visual_grid const * const grid)
{
	free(grid->vals_y);
	free(grid->vals_x);
	free(grid->vals_humi);
	free(grid->vals_temp);
}

#ifdef NCURSES
#include <ncursesw/ncurses.h>

BOOL terra_show(terra_visual_cmd const * const cmd, char const * const title, terra_data_entry const * const entries, size_t const count)
{
	terra_visual_mode mode = TERRA_BOTH;
	terra_visual_grid grid;
	int width;
	int height;
	int key;
	BOOL reload = FALSE;

	setlocale(LC_CTYPE, "en_US.UTF-8");

	initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(FALSE);

	start_color();
	init_pair(1, COLOR_DEFAULT, COLOR_BLACK);
	init_pair(2, COLOR_GRID, COLOR_BLACK);
	init_pair(3, COLOR_LEGEND, COLOR_BLACK);
	init_pair(4, COLOR_TEMP, COLOR_BLACK);
	init_pair(5, COLOR_HUMI, COLOR_BLACK);
	init_pair(6, COLOR_BOTH, COLOR_BLACK);

	getmaxyx(stdscr, height, width);

	while (1)
	{
		terra_visual_grid_init(&grid, cmd, width, height, entries, count, mode);

		terra_visual_draw_title(&grid, title, width, count, mode);
		terra_visual_draw_grid(&grid, width, height, mode);
		terra_visual_draw_labels(width, height, &grid);

		key = getch();

		terra_visual_grid_free(&grid);

		if (key == KEY_EXIT || key == KEY_ESC)
			break;

		if (key == KEY_F5)
		{
			reload = TRUE;
			break;
		}

		show_shift_mode(&mode);
		clear();
	}

	endwin();
	return reload;
}

#endif

BOOL terra_visual_show(terra_visual_cmd const * const cmd)
{
	terra_data_entry *entries;
	size_t count;
	char path[50];
	BOOL reload;

	while (1)
	{
		if (!terra_data_read(&entries, &count, path, cmd->days_past))
		{
			terra_log_error("[terra_visual_show] failed to read entries\n");
			return FALSE;
		}

#ifdef NCURSES
		reload = terra_show(cmd, path, entries, count);
#else
		terra_log_info("[terra_visual_show] ncurses disabled\n");
#endif

		free(entries);

		if (!reload)
			break;
	}

	return TRUE;
}
