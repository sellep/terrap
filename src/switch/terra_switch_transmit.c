#include "terra_switch.h"

#include "../common_dht_read.h"

#define SIGNAL_PULSE_LEN 350

#define SIGNAL_SYNC_HIGH 1
#define SIGNAL_SYNC_LOW 31

#define SIGNAL_ZERO_HIGH 1
#define SIGNAL_ZERO_LOW 3

#define SIGNAL_ONE_HIGH 3
#define SIGNAL_ONE_LOW 1

static inline sleep_microseconds(size_t const us)
{
	struct timespec sleeper;
	size_t sec;
	size_t usec;

	if (us == 0)
		return;

	usec = us % 1000000;
	sec = us / 1000000;

	sleeper.tv_sec = sec;
	sleeper.tv_nsec = usec * 1000L;
	nanosleep(&sleeper, NULL);
}

static inline void switch_transmit(ssize_t const pin, ssize_t const high, ssize_t const low)
{
#ifndef DEBUG
	pi_2_mmio_set_high(pin);
	sleep_microseconds(SIGNAL_PULSE_LEN * high);
	pi_2_mmio_set_low(pin);
	sleep_microseconds(SIGNAL_PULSE_LEN * low);
#endif
}

void terra_switch_transmit(int const code)
{
	ssize_t rep;
	int i;

	LOCK();

	for (rep = 0; rep < CONF_SWITCH.repeats; rep++)
	{
		for (i = CONF_SWITCH.channel - 1; i >= 0; i--)
		{
			if (code & (1L << i))
			{
				switch_transmit(CONF_SWITCH.pin, SIGNAL_ONE_HIGH, SIGNAL_ONE_LOW);
			}
			else
			{
				switch_transmit(CONF_SWITCH.pin, SIGNAL_ZERO_HIGH, SIGNAL_ZERO_LOW);
			}
		}

		switch_transmit(CONF_SWITCH.pin, SIGNAL_SYNC_HIGH, SIGNAL_SYNC_LOW);
	}

	UNLOCK();
}
