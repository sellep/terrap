#include "terra_switch.h"

#include <sdio.h>

void terra_switch_set_pin(ssize_t const tran_pin)
{
#if WPI_ENABLED
	pinMode(tran_pin, OUTPUT);
#else
	printf("[terra_switch_set_pin] wpi disabled!\n");
#endif
}