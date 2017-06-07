#include "terra_time.h"

void terra_time_print(terra_time const * const time)
{
	if (time->hour < 10)
	{
		printf("0");
	}

	printf("%u:", time->hour);

	if (time->min < 10)
	{
		printf("0");
	}

	printf("%u:", time->min);

	if (time->sec < 10)
	{
		printf("0");
	}

	if (time->sec < 100)
	{
		printf("0");
	}

	printf("%u\n", time->sec);
}