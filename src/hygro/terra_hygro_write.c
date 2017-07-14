#include "terra_hygro.h"

BOOL terra_hygro_write(float const h, float const t, terra_time const * const now)
{
	terra_data_entry entry;
	BOOL res;

	terra_time_cpy(&entry.tm, now);
	entry.humi = h;
	entry.temp = t;

	res = terra_data_append(&entry);
	if (!res)
	{
		terra_log_error("[terra_hygro_write] failed to append entry\n");
	}

	return res;
}