#include "terra.h"

#ifdef WIRINGPI

#include <wiringPi.h>

#endif

void terra_led_set(ssize_t const pin, BOOL const on)
{
#ifdef WIRINGPI
	LOCK();
	digitalWrite(pin, on ? 1 : 0);
	UNLOCK();
#else
	terra_log_info("[terrad_led_set] wiringPi disabled!\n");
#endif
}
