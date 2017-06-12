#include "terra_hygro.h"

#include "../utils/terra_time.h"

#define DHT_MAXCOUNT 32000
#define DHT_PULSES 41

static inline BOOL dht_sync(ssize_t const pin)
{
	volatile ssize_t x;
	size_t i = 0;

#ifndef DEBUG
	pi_2_mmio_set_output(pin);

	pi_2_mmio_set_high(pin);
	sleep_milliseconds(500);

	pi_2_mmio_set_low(pin);
	busy_wait_milliseconds(20);

	pi_2_mmio_set_input(pin);

	//short delay
	for (x = 0; x < 50; x++) { }

	while (pi_2_mmio_input(pin))
	{
		if (++i < DHT_MAXCOUNT)
			continue;

		set_default_priority();
		return FALSE;
	}
#endif

	return TRUE;
}

static inline BOOL dht_record_pulse(ssize_t const pin, size_t * const pulses)
{
	ssize_t i;

#ifndef DEBUG
	for (i = 0; i < DHT_PULSES * 2; i += 2)
	{
		while (!pi_2_mmio_input(pin))
		{
			if (++pulses[i] < DHT_MAXCOUNT)
				continue;

			return FALSE;
		}

		while (pi_2_mmio_input(pin))
		{
			if (++pulses[i + 1] < DHT_MAXCOUNT)
				continue;

			return FALSE;
		}
	}
#endif

	return TRUE;
}

static inline size_t pulses_avg_low(size_t const * const pulses)
{
	size_t avg = 0;
	ssize_t i;

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
	ssize_t i;

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

static inline BOOL dht_verify_checksum(uint8_t * const data)
{
	if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
		return FALSE;
	return TRUE;
}

hygro_err terra_hygro_read(ssize_t const pin, float * const humidity, float * const temperature)
{
	size_t pulses[DHT_PULSES * 2] = { 0 };
	uint8_t data[5] = {0};

	set_max_priority();

	if (!dht_sync(pin))
		return HYGRO_ERROR_TIMEOUT;

	if (!dht_record_pulse(pin, pulses))
	{
		set_default_priority();
		return HYGRO_ERROR_TIMEOUT;
	}

	set_default_priority();

	pulses_to_data(pulses, data);

	if (!dht_verify_checksum(data))
		return HYGRO_ERROR_CHECKSUM;

	*humidity = (data[0] * 256 + data[1]) / 10.0f;
	*temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;

	if (data[2] & 0x80)
	{
		*temperature *= -1.0f;
	}

	return HYGRO_SUCCESS;
}

BOOL terra_hygro_read_rep(terra_conf const * const conf, float * const h, float * const t)
{
	ssize_t r;

	for (r = 0; r < conf->hygro_rep; r++)
	{
		if (terra_hygro_read(conf->hygro_pin_io, h, t) == HYGRO_SUCCESS)
		{
			terra_log_info("hygro humidity: %f.2, temperature: %f.2\n", *h, *t);
			return TRUE;
		}
	}

	return FALSE;
}