#include "terra_led.h"

#ifdef WIRINGPI

#include <wiringPi.h>

#endif

void terra_led_set(ssize_t const pin, terra_led_cmd const set)
{
#ifdef WIRINGPI
	LOCK();
	digitalWrite(pin, set & LED_ON ? 1 : 0);
	UNLOCK();
#else
	terra_log_info("[terrad_led_set] wiringPi disabled!\n");
#endif
}
