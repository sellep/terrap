#include "terra_visual.h"

static void grid_bounding_y(float * const min_y, float * const max_y, terra_visual grid * const grid, terra_data_entry const * const entries, size_t const len, terra_visual_mode const mode)
{
	size_t i;

	if (mode == TERRA_TEMP)
	{
		max_y[0] = entries[0].temp;
		min_y[0] = entries[0].temp;

		for (i = 1; i < len; i++)
		{
			if (entries[i].temp > max_y[0])
			{
				max_y[0] = entries[i].temp;
			}
			else if (entries[i].temp < min_y[0])
			{
				min_y[0] = entries[i].temp;
			}
		}

		grid->max_temp = max_y[0];
		grid->min_temp = min_y[0];
	}
	else if (mode == TERRA_HUMI)
	{
		max_y[0] = entries[0].humi;
		min_y[0] = entries[0].humi;

		for (i = 1; i < len; i++)
		{
			if (entries[i].humi > max_y[0])
			{
				max_y[0] = entries[i].humi;
			}
			else if (entries[i].humi < min_y[0])
			{
				min_y[0] = entries[i].humi;
			}
		}

		grid->max_humi = max_y[0];
		grid->min_humi = min_y[0];
	}
	else
	{
		if (entries[0].humi > entries[0].temp)
		{
			max_y[0] = entries[0].humi;
			min_y[0] = entries[0].temp;
		}
		else
		{
			min_y[0] = entries[0].humi;
			max_y[0] = entries[0].temp;
		}

		grid->max_humi = entries[0].humi;
		grid->min_humi = entries[0].humi;
		grid->max_temp = entries[0].temp;
		grid->max_temp = entries[0].temp;

		for (i = 1; i < len; i++)
		{
			if (entries[i].humi > entries[i].temp)
			{
				if (entries[i].humi > max_y[0])
				{
					max_y[0] = entries[i].humi;
					grid->max_humi = entries[i].humi;
				}
				if (entries[i].temp < min_y[0])
				{
					min_y[0] = entries[i].temp;
					grid->min_temp = entries[i].temp;
				}
			}
			else
			{
				if (entries[i].temp > max_y[0])
				{
					max_y[0] = entries[i].temp;
					grid->max_temp = entries[i].temp;
				}
				if (entries[i].humi < min_y[0])
				{
					min_y[0] = entries[i].humi;
					grid->min_humi = entries[i].humi;
				}
			}
		}
	}
}

static inline void compute_vals_y(terra_visual_grid * const grid, ssize_t const height, terra_data_entry const * const entries, size_t const len, terra_visual_mode const mode)
{
	float min;
	float max;
	double step;
	double start;
	size_t y;

	grid_bounding_y(&min, &max, grid, entries, len, mode);

	step = (double)(max - min) / (DRAW_HEIGHT - 3);
	start = min - step;

	for (y = 0; y < DRAW_HEIGHT; y++)
	{
		grid->vals_y[y] = (float)(start + y * step);
	}
}

static inline void compute_vals_x(terra_visual_grid * const grid, ssize_t const width, terra_data_entry const * const entries, size_t const len)
{
	double step;
	size_t start;
	size_t x;

	step = (double)terra_time_diff(&entries[len - 1].tm, &entries[0].tm) / DRAW_WIDTH;
	start = terra_time_to_int(&entries[0].tm);

	for (x = 0; x < DRAW_WIDTH; x++)
	{
		grid->vals_x[x] = (size_t) (start + x * step);
	}
}

static inline eval_entries_humi(terra_visual_grid * const grid, ssize_t const height, terra_data_entry const * const entry, ssize_t const x)
{
	ssize_t y;

	for (y = 1; y < DRAW_HEIGHT; y++)
	{
		if (grid->vals_y[y] < entry->humi)
			continue;
	
		grid->vals_humi[x] = y;
		break;
	}
}

static inline eval_entries_temp(terra_visual_grid * const grid, ssize_t const height, terra_data_entry const * const entry, ssize_t const x)
{
	ssize_t y;

	for (y = 1; y < DRAW_HEIGHT; y++)
	{
		if (grid->vals_y[y] < entry->temp)
			continue;
	
		grid->vals_temp[x] = y;
		break;
	}
}

static inline void eval_entries(terra_visual_grid * const grid, ssize_t const width, ssize_t const height, terra_data_entry const * const entries, size_t const len, terra_visual_mode const mode)
{
	terra_data_entry *entry;
	ssize_t x;

	if (mode == TERRA_BOTH)
	{
		for (x = 0; x < DRAW_WIDTH; x++)
		{
			entry = &entries[x * len / (DRAW_WIDTH - 1)];

			eval_entries_humi(grid, height, entry, x);
			eval_entries_temp(grid, height, entry, x);
		}
	}
	else if (mode == TERRA_HUMI)
	{
		for (x = 0; x < DRAW_WIDTH; x++)
		{
			entry = &entries[x * len / (DRAW_WIDTH - 1)];
	
			eval_entries_humi(grid, height, entry, x);
		}
	}
	else
	{
		for (x = 0; x < DRAW_WIDTH; x++)
		{
			entry = &entries[x * len / (DRAW_WIDTH - 1)];
	
			eval_entries_temp(grid, height, entry, x);
		}
	}
}

void terra_visual_grid_init(terra_visual_grid * const grid, ssize_t const width, ssize_t const height, terra_data_entry const * const entries, size_t const len, terra_visual_mode const mode)
{
	grid->vals_y = (float*) malloc(sizeof(float) * DRAW_HEIGHT);
	grid->vals_x = (size_t*) malloc(sizeof(size_t) * DRAW_WIDTH);
	grid->vals_humi = (ssize_t*) malloc(sizeof(size_t) * DRAW_WIDTH);
	grid->vals_temp = (ssize_t*) malloc(sizeof(size_t) * DRAW_WIDTH);

	compute_vals_y(grid, height, entries, len, mode);
	compute_vals_x(grid, width, entries, len);

	eval_entries(grid, width, height, entries, len, mode);
}
