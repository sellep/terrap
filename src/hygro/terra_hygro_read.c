#include "terra_hygro.h"

//https://github.com/technion/lol_dht22/blob/master/dht22.c

#ifdef WPI_ENABLED

#include <wiringPi.h>

#define MAX_TIMINGS 85
#define MIN_BITS_READ 40
#define FIRST_IGNORABLE_TRANSITIONS 3
#define IS_EVEN(i)(i % 2 == 0)
#define NEG(t)(t *= -1)

static int _dht22_data[5] = { 0, 0, 0, 0, 0 };

#define CHECKSUM_BYTE (_dht22_data[4])

static void terra_hygro_read_sync(ssize_t const pin)
{
	pinMode(pin, OUTPUT);

	digitalWrite(pin, HIGH);
	delay(10);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
}

static ssize_t assert_data_read(const int read)
{
	if (read > 255 || read < 0)
	{
		terra_log_error("CRITICAL: invalid data from wiringPi\n");
	}
	return (ssize_t)read;
}

static ssize_t terra_hygro_read_impl(ssize_t const pin)
{
	ssize_t i;
	ssize_t j = 0;
	ssize_t counter;
	ssize_t last = HIGH;

	pinMode(pin, INPUT);

	for (i = 0; i < MAX_TIMINGS; i++)
	{
		counter = 0;

		while (assert_data_read(digitalRead(pin)) == last)
		{
			counter++;
			delayMicroseconds(1);

			if (counter == 255)
				break;
		}

		last = assert_data_read(digitalRead(pin));

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

static BOOL terra_hygro_read_convert(terra_hygro_res * const res, ssize_t const j)
{
	float t;
	float h;

	if (j < MIN_BITS_READ)
		return FALSE;

	if (CHECKSUM_BYTE != ((_dht22_data[0] + _dht22_data[1] + _dht22_data[2] + _dht22_data[3]) & 0xFF))
		return FALSE;

	h = (float)_dht22_data[0] * 256 + (float)_dht22_data[1];
	h /= 10;
	t = (float)(_dht22_data[2] & 0x7F)* 256 + (float)_dht22_data[3];
	t /= 10;

	if ((_dht22_data[2] & 0x80) != 0)
	{
		NEG(t);
	}

	res->temp = t;
	res->humi = h;

	return TRUE;
}

#endif

BOOL terra_hygro_read(terra_hygro_res * const res, terra_conf const * const conf)
{
	BOOL result;

#ifdef WPI_ENABLED
	ssize_t j;

	LOCK();

	terra_hygro_read_sync(conf->hygro_pin_io);
	j = terra_hygro_read_impl(conf->hygro_pin_io);

	UNLOCK();

	result = terra_hygro_read_convert(res, j);
#else
	result = FALSE;
#endif

	return result;
}
