#include "terra_switch.h"

#define SIGNAL_PULSE_LEN 350

void terra_switch_transmit(ssize_t const pin, ssize_t const high, ssize_t const low, ssize_t const pulse_len)
{
	pi_2_mmio_set_high(pin);
	sleep_microseconds(SIGNAL_PULSE_LEN * high);
	pi_2_mmio_set_low(pin);
	sleep_microseconds(SIGNAL_PULSE_LEN * low);
}
