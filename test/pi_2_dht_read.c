#include <stdbool.h>
#include <stdlib.h>

#include "pi_2_dht_read.h"
#include "pi_2_mmio.h"

#define DHT_MAXCOUNT 32000

#define DHT_PULSES 41

#define DELAY() ( \
	volatile ssize_t x; \
	for (x = 0; x < 50; x++) { })

static inline int dht_sync(ssize_t const pin)
{
	size_t i = 0;

	pi_2_mmio_set_output(pin);

	pi_2_mmio_set_high(pin);
	sleep_milliseconds(500);

	pi_2_mmio_set_low(pin);
	busy_wait_milliseconds(20);

	pi_2_mmio_set_input(pin);
	DELAY();

	while (pi_2_mmio_input(pin))
	{
		if (++i < DHT_MAXCOUNT)
			continue;

		set_default_priority();
		return DHT_ERROR_TIMEOUT;
	}

	return DHT_SUCCESS;
}

static inline int dht_record_pulse(ssize_t const pin, size_t * const pulses)
{
	ssize_t i;

	for (i = 0; i < DHT_PULSES * 2; i += 2)
	{
		while (!pi_2_mmio_input(pin))
		{
			if (++pulses[i] < DHT_MAXCOUNT)
				continue;

			return DHT_ERROR_TIMEOUT;
		}

		while (pi_2_mmio_input(pin))
		{
			if (++pulses[i + 1] < DHT_MAXCOUNT)
				continue;

			return DHT_ERROR_TIMEOUT;
		}
	}

	return DHT_SUCCESS;
}

static inline size_t pulses_avg_low(size_t const * const pulses)
{
	size_t avg = 0;
	ssize_t i;

	//ignore the first two readings since they are a constant 80 microsecond pulse.
	for (i = 2; i < DHT_PULSES * 2; i += 2)
	{
		avg += pulses[i];
	}

	return (avg / DHT_PULSES - 1);
}

static inline void pulses_to_data(size_t const * const pulses, uint8_t * const data)
{
	size_t avg_low;
	ssize_t idx;

	avg_low = pulses_avg_low(pulses);

	for (i = 3; i < DHT_PULSES * 2; i += 2)
	{
		idx = (i - 3) / 16;
		data[idx] <<= 1;

		if (pulses[i] >= avg_low)
		{
			data[idx] |= 1;
		}
	}
}

static inline int dht_verify_checksum()
{
	if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
		return DHT_ERROR_CHECKSUM;
	return DHT_SUCCESS;
}

int dht_read(ssize_t const pin, float * const humidity, float * const temperature)
{
	size_t pulses[DHT_PULSES * 2] = { 0 };
	uint8_t data[5] = {0};
	size_t avg_low;
	size_t count = 0;
	size_t i, j;

	set_max_priority();

	if (dht_sync(pin) < 0)
		return DHT_ERROR_TIMEOUT;

	if (dht_record(pin, pulses) < 0)
	{
		set_default_priority();
		return DHT_ERROR_TIMEOUT;
	}

	pulses_to_data(pulses, data);

	if (dht_verify_checksum < 0)
		return DHT_ERROR_CHECKSUM;

	*humidity = (data[0] * 256 + data[1]) / 10.0f;
	*temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;

	if (data[2] & 0x80)
	{
		*temperature *= -1.0f;
	}

	return DHT_SUCCESS;
}