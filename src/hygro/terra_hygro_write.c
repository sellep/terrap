#include "terra_hygro.h"

#include "../utils/terra_data.h"

BOOL terra_hygro_write(float const h, float const t)
{
	terra_data_entry entry;
	BOOL res;

	terra_time_cpy(&entry.tm, &runtime.now);
	entry.humi = h;
	entry.temp = t;

	res = terra_data_append(&entry);
	if (!res)
	{
		terra_log_error("[terra_hygro_write] failed to append entry\n");
	}
	else
	{
		terra_time_cpy(&hygro_last, &runtime.now);
	}

	return res;
}