#include "terra_pin.h"

void terra_pin_set_output(ssize_t const pin)
{
	LOCK();
#ifndef DEBUG
	pi_2_mmio_set_output(pin);
#endif
	UNLOCK();
}
