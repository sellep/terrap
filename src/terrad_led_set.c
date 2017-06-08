#include "terra.h"

void terrad_led_set(ssize_t const pin, BOOL const on)
{
#ifdef WPI_ENABLED
	LOCK();
	digitalWrite(pin, on ? 1 : 0);
	UNLOCK();
#else
	terra_log_info("[terrad_led_set] wiringPi disabled!\n");
#endif
}