#include "terra_visual.h"

static inline void grid_bounding_y(terra_data_entry const * const entries, size_t const len, float * const min_y, float * const max_y)
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

static inline void compute_vals_y(terra_visual_grid * const grid, size_t const height, terra_data_entry const * const entries, size_t const len)
{
	float min;
	float max;
	double step;
	double start;
	size_t y;

	grid_bounding_y(entries, len, &min, &max);

	step = ((double) max - min) / (DRAW_HEIGHT() - 2);
	start = min - step;

	for (y = 0; y < DRAW_HEIGHT(); y++)
	{
		grid->vals_y[y] = (float)(start + y * step);
	}
}

static inline void compute_vals_x(terra_visual_grid * const grid, size_t const width, terra_data_entry const * const entries, size_t const len)
{
	double step;
	size_t start;
	size_t x;

	step = (double)terra_time_diff(&entries[0].tm, &entries[len - 1].tm) / DRAW_WIDTH();
	start = terra_time_to_int(&entries[0].tm);

	for (x = 0; x < DRAW_WIDTH(); x++)
	{
		grid->vals_x[x] = (size_t) (start + x * step);
	}
}

void terra_visual_grid_init(terra_visual_grid * const grid, size_t const width, size_t const height, terra_data_entry const * const entries, size_t const len)
{
	grid->vals_y = (float*) malloc(sizeof(float) * DRAW_HEIGHT());
	grid->vals_x = (size_t*) malloc(sizeof(size_t) * DRAW_WIDTH());

	compute_vals_y(grid, height, entries, len);
	compute_vals_x(grid, width, entries, len);
}
