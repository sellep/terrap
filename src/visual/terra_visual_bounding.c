#include "terra_visual.h"

void terra_visual_bounding(terra_visual_bounds * const bounds, terra_data_entry const * const entries, size_t const len)
{
	size_t i;

	bounds->ymin = entries[0].humi < entries[0].temp ? entries[0].humi : entries[0].temp;
	bounds->ymax = entries[0].humi > entries[0].temp ? entries[0].humi : entries[0].temp;
	bounds->xmin = entries[0].tm;
	bounds->xmax = entries[len - 1].tm;
	bounds->temp_min = entries[0].temp;
	bounds->temp_max = entries[0].temp;
	bounds->humi_min = entries[0].humi;
	bounds->humi_max = entries[0].humi;

	for (i = 1; i < len; i++)
	{
		if (entries[i].humi > bounds->humi_max)
		{
			bounds->humi_max = entries[i].humi;

			if (bounds->humi_max > bounds->ymax)
			{
				bounds->ymax = bounds->humi_max;
			}
		}
		else if (entries[i].humi < bounds->humi_min)
		{
			bounds->humi_min = entries[i].humi;

			if (bounds->humi_min < bounds->ymin)
			{
				bounds->ymin = bounds->humi_min;
			}
		}

		if (entries[i].temp > bounds->temp_max)
		{
			bounds->temp_max = entries[i].temp;

			if (bounds->temp_max > bounds->ymax)
			{
				bounds->ymax = bounds->temp_max;
			}
		}
		else if (entries[i].temp < bounds->temp_min)
		{
			bounds->temp_min = entries[i].temp;

			if (bounds->temp_max < bounds->ymin)
			{
				bounds->ymin = bounds->temp_min;
			}
		}
	}
}