#include <stdlib.h>
#include <stdio.h>

#include "pi_2_dht_read.h"
#include "common_dht_read.h"

#define PIN 27


int main()
{
	float t;
	float h;

	int status;

	if (pi_2_mmio_init() < 0)
		return DHT_ERROR_GPIO;

	do
	{
		printf("pi_2_dht_read\n");
		status = pi_2_dht_read(DHT22, PIN, &h, &t);
	}
	while (status != DHT_SUCCESS);

	printf("temperature: %f.2, humidity: %f.2\n", t, h);
	return 0;
}
