#include "terra_time.h"

void terra_time_print(terra_time const * const time)
{
	if (time->hour < 10)
	{
		terra_log_info("0");
	}

	terra_log_info("%u:", time->hour);

	if (time->min < 10)
	{
		terra_log_info("0");
	}

	terra_log_info("%u:", time->min);

	if (time->sec < 10)
	{
		terra_log_info("0");
	}

	terra_log_info("%u\n", time->sec);
}
