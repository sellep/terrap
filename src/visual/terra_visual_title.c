#include "terra_visual.h"

#define DETAILS_FORMAT "entries: %zu, temp: %.2f - %.2f, humi: %.2f - %.2f"

void terra_visual_title(char const * const title, ssize_t const width, size_t const entries, terra_visual_bounds const * const bounds)
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

	mvprintw(2, 1, DETAILS_FORMAT, entries, bounds->temp_min, bounds->temp_max, bounds->humi_min, bounds->humi_max);
#endif
}