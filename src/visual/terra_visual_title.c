#include "terra_visual.h"

void terra_visual_title(char const * const title, ssize_t const width)
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