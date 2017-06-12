#include "terra_led.h"

void terra_led_set(ssize_t const pin, terra_led_cmd const set)
{
	LOCK();

	if (set & LED_ON)
	{
#ifndef DEBUG
		pi_2_mmio_set_high(pin);
#else
		terra_log_info("[terra_led_set] set %zu on\n", pin);
#endif
	}
	else
	{
#ifndef DEBUG
		pi_2_mmio_set_low(pin);
#else
		terra_log_info("[terra_led_set] set %zu off\n", pin);
#endif
	}

	UNLOCK();
}
