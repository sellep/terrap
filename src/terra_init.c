#include "terra.h"

#include "utils/terra_pin.h"
#include "led/terra_led.h"

BOOL terra_init()
{
#ifndef DEBUG
	if (pi_2_mmio_init() != MMIO_SUCCESS)
	{
		terra_log_error("[terra_init] failed to initialize mmio\n");
		return FALSE;
	}

	if (!terra_lock_init())
	{
		terra_log_error("[terra_init] failed to initialize terra lock\n");
		return FALSE;
	}
#endif

	terra_pin_set_output(LED_PIN_ERR);
	return TRUE;
}
