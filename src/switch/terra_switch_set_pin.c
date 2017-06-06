#include "terra_switch.h"

void terra_switch_set_pin(ssize_t const tran_pin)
{
	pinMode(tran_pin, OUTPUT);
}