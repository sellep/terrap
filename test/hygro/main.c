#include "pi_2_dht_read.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * * argv)
{
	int pin;
	int status;
	float h, t;

	if (argc != 2)
	{
		sprintf(stderr, "insufficient arguments\n");
		exit(1);
	}

	pin = atoi(argv[1]);

	if (pin <= 0)
	{
		sprintf(stderr, "invalid pin\n");
		exit(1);
	}

	if ((status = pi_2_dht_read(DHT22, pin, &h, &t)) != DHT_SUCCESS)
	{
		sprintf(stderr, "pi_2_dht_read error(%i)\n", status);
		exit(1);
	}

	printf("humi: %.2f, temp: %.2f\n", h, t);
	return 0;
}
