#include "terra.h"

#include "utils/terra_pin.h"
#include "led/terra_led.h"

BOOL terra_init()
{
	mmio_status mstat;

#ifndef DEBUG
	printf("init mmio\n");
	if ((mstat = pi_2_mmio_init()) != MMIO_SUCCESS)
	{
		terra_log_error("failed to initialize mmio\n");
		return FALSE;
	}
#else
	printf("skipping init mmio\n");
#endif

printf("skipping init lock\n");
	if (!terra_lock_init())
	{
		terra_log_error("failed to initialize terra lock\n");
		return FALSE;
	}
printf("terra_pin_set_output\n");
	terra_pin_set_output(LED_PIN_ERR);
	return TRUE;
}
