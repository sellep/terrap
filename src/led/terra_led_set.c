#include "terra_led.h"

void terra_led_set(ssize_t const pin, terra_led_cmd const set)
{
	LOCK();

	if (set & LED_ON)
	{
#ifndef DEBUG
		pi_2_mmio_set_high(pin);
#endif
	}
	else
	{
#ifndef DEBUG
		pi_2_mmio_set_low(pin);
#endif
	}

	UNLOCK();
}
