#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>

#define MAX_TIMINGS 85
#define MIN_BITS_READ 40
#define FIRST_IGNORABLE_TRANSITIONS 3
#define IS_EVEN(i)(i % 2 == 0)
#define NEG(t)(t *= -1)

#define PIN 3

static int _dht22_data[5] = { 0, 0, 0, 0, 0 };

#define CHECKSUM_BYTE (_dht22_data[4])

static ssize_t dat_cnv(int const read)
{
	if (read > 255 || read < 0)
	{
		printf("CRITICAL: invalid data: %i\n", read);
		exit(1);
	}
	return (ssize_t)read;
}

static void hygro_sync()
{
	pinMode(PIN, OUTPUT);

	digitalWrite(PIN, HIGH);
	delay(10);
	digitalWrite(PIN, LOW);
	delay(18);
	digitalWrite(PIN, HIGH);
	delayMicroseconds(40);
}

static ssize_t hygro_read()
{
	ssize_t i;
	ssize_t j = 0;
	ssize_t counter;
	ssize_t last = HIGH;

	pinMode(PIN, INPUT);

	for (i = 0; i < MAX_TIMINGS; i++)
	{
		counter = 0;

		while (dat_cnv(digitalRead(PIN)) == last)
		{
			counter++;
			delayMicroseconds(1);

			if (counter == 255)
				break;
		}

		last = dat_cnv(digitalRead(PIN));

		if (counter == 255)
			break;

		if (i > FIRST_IGNORABLE_TRANSITIONS && IS_EVEN(i))
		{
			//shove each bit into the storage bytes
			_dht22_data[j / 8] <<= 1;

			if (counter > 16)
			{
				_dht22_data[j / 8] |= 1;
			}

			j++;
		}
	}

	return j;
}

static int hygro_cnv(ssize_t const j)
{
	float t;
	float h;

	if (j < MIN_BITS_READ)
		return 0;

	if (CHECKSUM_BYTE != ((_dht22_data[0] + _dht22_data[1] + _dht22_data[2] + _dht22_data[3]) & 0xFF))
{
	printf("checksum failed\n");
	return 0;
}

	h = (float)_dht22_data[0] * 256 + (float)_dht22_data[1];
	h /= 10;
	t = (float)(_dht22_data[2] & 0x7F)* 256 + (float)_dht22_data[3];
	t /= 10;

	if ((_dht22_data[2] & 0x80) != 0)
	{
		NEG(t);
	}

	printf("temperature: %f.2,  humidity: %f.2", t, f);
	return 1;
}

int main()
{
	ssize_t v;
	int b;

	do
	{
		hygro_sync();
		v = hygro_read();
		b = hygro_cnv(v);
	}
	while (!b);	

	return 0;
}
