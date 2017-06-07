#include "terra_switch.h"

#define SIGNAL_PULSE_LEN 350

void terra_switch_transmit(ssize_t const tran_pin, ssize_t const high, ssize_t const low, ssize_t const pulse_len)
{
#ifdef WPI_ENABLED
	digitalWrite(tran_pin, HIGH);
	delayMicroseconds(SIGNAL_PULSE_LEN * high);
	digitalWrite(tran_pin, LOW);
	delayMicroseconds(SIGNAL_PULSE_LEN * low);
#endif
}