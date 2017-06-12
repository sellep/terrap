#include "terra.h"

#include "utils/terra_pin.h"
#include "led/terra_led.h"

BOOL terra_init()
{
	mmio_status mstat;

#ifndef DEBUG
	if ((mstat = pi_2_mmio_init()) != MMIO_SUCCESS)
	{
		terra_log_error("failed to initialize mmio\n");
		return FALSE;
	}
#endif


	if (!terra_lock_init())
	{
		terra_log_error("failed to initialize terra lock\n");
		return FALSE;
	}

	terra_pin_set_output(LED_PIN_ERR);
	printf("terra_pin_set_output finished\n");
	return TRUE;
}
