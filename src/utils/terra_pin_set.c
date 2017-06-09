#include "terra_pin.h"

#ifdef WIRINGPI

#include <wiringPi.h>

#endif

void terra_pin_set_output(ssize_t const pin)
{
#ifdef WIRINGPI
	LOCK();
	pinMode(pin, OUTPUT);
	UNLOCK();
#else
	terra_log_info("[terra_pin_set_output] wiringPi disabled!\n");
#endif
}
