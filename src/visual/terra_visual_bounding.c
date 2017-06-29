#include "terra_visual.h"

void terra_visual_bounding(terra_visual_bounds * const bounds, terra_data_entry const * const entries, size_t const len)
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

	printf("bounds ymin: %.2f", bounds->ymin);
	printf("bounds ymax: %.2f", bounds->ymax);
	terra_time_print(&bounds->xmin);
	terra_time_print(&bounds->xmax);
}