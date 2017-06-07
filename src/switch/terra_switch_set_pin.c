#include "terra_switch.h"

void terra_switch_set_pin(ssize_t const tran_pin)
{
#ifdef WPI_ENABLED
	pinMode(tran_pin, OUTPUT);
#else
	terra_log_info("[terra_switch_set_pin] wpi disabled!\n");
#endif
}