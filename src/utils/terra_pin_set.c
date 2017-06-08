#include "terra_pin.h"

void terra_pin_set_output(ssize_t const pin)
{
#ifdef WPI_ENABLED
	LOCK();
	pinMode(tran_pin, OUTPUT);
	UNLOCK();
#else
	terra_log_info("[terra_pin_set_output] wiringPi disabled!\n");
#endif
}

void terra_pin_set_input(ssize_t const pin)
{
#ifdef WPI_ENABLED
	LOCK();
	pinMode(tran_pin, INPUT);
	UNLOCK();
#else
	terra_log_info("[terra_pin_set_input] wiringPi disabled!\n");
#endif
}