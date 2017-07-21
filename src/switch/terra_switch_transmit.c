#include "terra_switch.h"

#define SIGNAL_PULSE_LEN 350

#define SIGNAL_SYNC_HIGH 1
#define SIGNAL_SYNC_LOW 31

#define SIGNAL_ZERO_HIGH 1
#define SIGNAL_ZERO_LOW 3

#define SIGNAL_ONE_HIGH 3
#define SIGNAL_ONE_LOW 1

static inline void sleep_microseconds(size_t const us)
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

void terra_switch_transmit(terra_conf_switch const * const conf, int const code)
{
	ssize_t rep;
	int i;

	LOCK();

	for (rep = 0; rep < conf->repeats; rep++)
	{
		for (i = conf->groups[0].channel - 1; i >= 0; i--)
		{
			if (code & (1L << i))
			{
				switch_transmit(conf->pin, SIGNAL_ONE_HIGH, SIGNAL_ONE_LOW);
			}
			else
			{
				switch_transmit(conf->pin, SIGNAL_ZERO_HIGH, SIGNAL_ZERO_LOW);
			}
		}

		switch_transmit(conf->pin, SIGNAL_SYNC_HIGH, SIGNAL_SYNC_LOW);
	}

	UNLOCK();
}
