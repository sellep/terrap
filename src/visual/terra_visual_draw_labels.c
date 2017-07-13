#include "terra_visual.h"

static inline ssize_t labels_remove_secs(char * const lbl)
{
	size_t i = 0;
	size_t c = 0;

	while (TRUE)
	{
		if (lbl[i++] != ':')
			continue;

		if (c++ == 0)
			continue;

		lbl[i - 1] = '\0';
		return i - 1;
	}
}

static inline void draw_labels_y(terra_visual_grid const * const grid)
{
	float step_y;
	size_t i;

	for (i = 0; i < grid->height; i++)
	{
		if (i % GRID_MARKER_Y == 0)
		{
			mvprintw(GRID_OFFSET_TOP + grid->height - (i + 1), 0, "%.1f", grid->vals_y[i]);
		}
	}
}

void terra_visual_draw_labels(ssize_t const width, ssize_t const height, terra_visual_grid const * const grid)
{
#ifdef NCURSES
	draw_labels_y(grid);
	/*float y_val;
	float y_step;

	terra_time tt;
	size_t x_min;
	size_t x_max;
	size_t x_val;
	size_t x_step;
	ssize_t mlen;
	char buf[10];

	
	ssize_t i;

	

	
	

	//x_min = terra_time_to_int(&bounds->xmin);
	//x_max = terra_time_to_int(&bounds->xmax);
    //
	//lbls = DRAW_WIDTH / GRID_MARKER_X;
	//if (lbls * GRID_MARKER_X < DRAW_WIDTH)
	//{
	//	lbls++;
	//}
    //
	//x_step = (x_max - x_min) / (lbls - 1);
	//x_val = x_min;
    //
	//for (i = 0; i < lbls; i++, x_val += x_step)
	//{
	//	terra_time_from_int(&tt, x_val);
	//	terra_time_to_arr(buf, &tt);
	//	mlen = labels_remove_secs(buf);
	//	mvprintw(height - GRID_OFFSET_BOTTOM, GRID_OFFSET_LEFT + i * GRID_MARKER_X - (mlen / 2), "%s", buf);
	//}*/
#endif
}
