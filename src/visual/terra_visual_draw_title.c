#include "terra_visual.h"

#define DETAILS_FORMAT "mode: %s, entries: %zu, temp: %.2f - %.2f, humi: %.2f - %.2f"

#define METADATA_FORMAT_BOTH "mode: BOTH, entries: %zu, humi: %.1f - %.1f, temp: %.1f - %.1f"
#define METADATA_FORMAT_HUMI "mode: HUMI, entries: %zu, humi: %.1f - %.1f"
#define METADATA_FORMAT_TEMP "mode: TEMP, entries: %zu, temp: %.1f - %.1f"

static char buf[256];

static inline void visual_draw_metadata(terra_visual_grid const * const grid, size_t const len, terra_visual_mode const mode)
{
	if (mode == TERRA_BOTH)
	{
		sprintf(buf, METADATA_FORMAT_BOTH, len, grid->min_humi, grid->max_humi, grid->min_temp, grid->max_temp);
	}
	else if (mode == TERRA_HUMI)
	{
		sprintf(buf, METADATA_FORMAT_HUMI, len, grid->min_humi, grid->max_humi);
	}
	else
	{
		sprintf(buf, METADATA_FORMAT_TEMP, len, grid->min_temp, grid->max_temp);
	}

	mvaddnstr(1, 0, buf, strlen(buf));
}

void terra_visual_draw_title(terra_visual_grid const * const grid, char const * const title, ssize_t const width, size_t const entries, terra_visual_mode const mode)
{
#ifdef NCURSES
	ssize_t title_len;
	ssize_t len;

	title_len = strlen(title);
	len = (width - title_len) / 2;

	memset(buf, '-', sizeof buf);
	strncpy(buf + len, title, title_len);
	mvaddnstr(0, 0, buf, width);

	visual_draw_metadata(grid, entries, mode);
#endif
}
