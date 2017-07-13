#include "terra_visual.h"

static inline void grid_bounding(terra_data_entry const * const entries, size_t const len, float * const min_y, float * const max_y)
{
	size_t i;

	max_y[0] = entries[0].humi > entries[0].temp ? entries[0].humi : entries[0].temp;
	min_y[0] = entries[0].humi < entries[0].temp ? entries[0].humi : entries[0].temp;

	for (i = 1; i < len; i++)
	{
		if (entries[i].humi > entries[i].temp)
		{
			if (entries[i].humi > max_y[0])
			{
				max_y[0] = entries[i].humi;
			}
			if (entries[i].temp < min_y[0])
			{
				min_y[0] = entries[i].temp;
			}
		}
		else
		{
			if (entries[i].temp > max_y[0])
			{
				max_y[0] = entries[i].temp;
			}
			if (entries[i].humi < min_y[0])
			{
				min_y[0] = entries[i].humi;
			}
		}
	}
} 

void terra_visual_grid_init(terra_visual_grid * const grid, size_t const width, size_t const height, terra_data_entry const * const entries, size_t const len)
{
	float min_y;
	float max_y;
	float step_y;
	float start_y;
	size_t i;

	grid->height = height - GRID_OFFSET_TOP - GRID_OFFSET_BOTTOM;
	grid->vals_y = (float*) malloc(sizeof(float) * grid->height);

	grid_bounding(entries, len, &min_y, &max_y);

	step_y = (max_y - min_y) / (grid->height - 2);
	start_y = min_y - step_y;

	mvprintw(1, 0, "min_y %f, max_y %f, step_y %f, start_y %f", min_y, max_y, step_y, start_y);
	
	for (i = 0; i < grid->height; i++)
	{
		grid->vals_y[i] = start_y + i * step_y;
	}
}
