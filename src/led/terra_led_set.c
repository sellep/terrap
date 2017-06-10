#include "terra_led.h"

void terra_led_set(ssize_t const pin, terra_led_cmd const set)
{
	LOCK();

	if (set & LED_ON)
	{
		pi_2_mmio_set_high(pin);
	}
	else
	{
		pi_2_mmio_set_low(pin);
	}

	UNLOCK();
}
