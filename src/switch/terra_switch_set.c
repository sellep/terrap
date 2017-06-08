#include "terra_switch.h"

#define SIGNAL_SYNC_HIGH 1
#define SIGNAL_SYNC_LOW 31

#define SIGNAL_ZERO_HIGH 1
#define SIGNAL_ZERO_LOW 3

#define SIGNAL_ONE_HIGH 3
#define SIGNAL_ONE_LOW 1

extern void terra_switch_transmit(ssize_t const, ssize_t const high, ssize_t const low);
extern size_t terra_switch_get_code(terra_conf const * const, terra_switch_req const * const);

void terra_switch_set(terra_conf const * const conf, terra_switch_req const * const req)
{
	ssize_t rep;
	ssize_t sock_code;
	int i;

	sock_code = terra_switch_get_code(conf, req);

	LOCK();

	for (rep = 0; rep < conf->switch_rep; rep++)
	{
		for (i = conf->switch_chan - 1; i >= 0; i--)
		{
			if (sock_code & (1L << i))
			{
				terra_switch_transmit(conf->switch_pin, SIGNAL_ONE_HIGH, SIGNAL_ONE_LOW);
			}
			else
			{
				terra_switch_transmit(conf->switch_pin, SIGNAL_ZERO_HIGH, SIGNAL_ZERO_LOW);
			}
		}

		terra_switch_transmit(conf->switch_pin, SIGNAL_SYNC_HIGH, SIGNAL_SYNC_LOW);
	}

	UNLOCK();
}