#include "terra_switch.h"

#ifdef WIRINGPI

#include <wiringPi.h>

#define SIGNAL_PULSE_LEN 350

#endif

void terra_switch_transmit(ssize_t const tran_pin, ssize_t const high, ssize_t const low, ssize_t const pulse_len)
{
#ifdef WIRINGPI
	digitalWrite(tran_pin, HIGH);
	delayMicroseconds(SIGNAL_PULSE_LEN * high);
	digitalWrite(tran_pin, LOW);
	delayMicroseconds(SIGNAL_PULSE_LEN * low);
#endif
}
