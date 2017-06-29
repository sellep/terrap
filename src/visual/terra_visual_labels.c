#include "terra_visual.h"

void terra_visual_labels(ssize_t const width, ssize_t const height, terra_visual_bounds const * const bounds)
{
#ifdef NCURSES
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

	lbls = (height - GRID_OFFSET_BOTTOM - GRID_OFFSET_TOP) / GRID_MARKER_Y + 1;
	step = (bounds->ymax - bounds->ymin) / (lbls - 1);
	y_val = bounds->ymin;

	for (i = 0; i < lbls; i++)
	{
		mvprintw(height - GRID_OFFSET_BOTTOM - i * GRID_MARKER_Y, 1, "%.1f", y_val);
		y_val += step;
	}

	x_min = terra_time_to_int(&bounds->xmin);
	x_max = terra_time_to_int(&bounds->xmax);

	lbls = (width - GRID_OFFSET_LEFT - GRID_OFFSET_RIGHT) / GRID_MARKER_X + 1;
	step = (x_max - x_min) / (lbls - 1);
	x_val = x_min;

	for (i = 0; i < lbls; i++)
	{
		terra_time_from_int(&tt, x_val);
		mlen = tttoa(buf, &tt);
		mvprintw(height - GRID_OFFSET_BOTTOM + 1, GRID_OFFSET_LEFT + i * GRID_MARKER_X - (mlen / 2), "%s", buf);
		x_val += step;
	}
#endif
}

