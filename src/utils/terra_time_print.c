#include "terra_time.h"

void terra_time_printnl(terra_time const * const time)
{
	if (time->hour < 10)
	{
		printf("0");
	}

	printf("%hu:", time->hour);

	if (time->min < 10)
	{
		printf("0");
	}

	printf("%hu:", time->min);

	if (time->sec < 10)
	{
		printf("0");
	}

	printf("%hu", time->sec);
}
