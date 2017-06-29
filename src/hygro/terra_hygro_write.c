#include "terra_hygro.h"

static terra_time _last_write = { 0, 0, 0 };

BOOL terra_hygro_write(terra_conf const * const conf, terra_time const * const sys, float const h, float const t)
{
	terra_data_entry entry;
	size_t diff;
	BOOL res;

	diff = terra_time_diff(sys, &_last_write);

	if (diff < conf->hygro_write_secs)
		return TRUE;

	terra_time_cpy(&entry.tm, sys);
	entry.humi = h;
	entry.temp = t;

	res = terra_data_append(&entry);
	if (!res)
	{
		terra_log_error("[terra_hygro_write] failed to append entry\n");
	}
	else
	{
		terra_time_cpy(&_last_write, sys);
	}

	return res;
}