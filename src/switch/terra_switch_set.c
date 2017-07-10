#include "terra_switch.h"

#define SIGNAL_PULSE_LEN 350

#define SIGNAL_SYNC_HIGH 1
#define SIGNAL_SYNC_LOW 31

#define SIGNAL_ZERO_HIGH 1
#define SIGNAL_ZERO_LOW 3

#define SIGNAL_ONE_HIGH 3
#define SIGNAL_ONE_LOW 1

static inline size_t switch_get_code(terra_switch_req const * const req)
{
	if (req->sock == 'a')
		return req->set_on ? conf.switch_code_aon : conf.switch_code_aoff;

	if (req->sock == 'b')
		return req->set_on ? conf.switch_code_bon : conf.switch_code_boff;

	return req->set_on ? conf.switch_code_con : conf.switch_code_coff;
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

void terra_switch_set(terra_switch_req const * const req)
{
	ssize_t rep;
	ssize_t sock_code;
	int i;

	sock_code = switch_get_code(req);

	terra_log_info("[terra_switch_set] set switch %c to %s\n", req->sock, req->set_on ? "on" : "off");

	LOCK();

	for (rep = 0; rep < conf.switch_rep; rep++)
	{
		for (i = conf.switch_chan - 1; i >= 0; i--)
		{
			if (sock_code & (1L << i))
			{
				switch_transmit(conf.switch_pin, SIGNAL_ONE_HIGH, SIGNAL_ONE_LOW);
			}
			else
			{
				switch_transmit(conf.switch_pin, SIGNAL_ZERO_HIGH, SIGNAL_ZERO_LOW);
			}
		}

		switch_transmit(conf.switch_pin, SIGNAL_SYNC_HIGH, SIGNAL_SYNC_LOW);
	}

	UNLOCK();
}