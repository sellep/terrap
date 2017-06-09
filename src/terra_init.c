#include "terra.h"

#ifdef WIRINGPI
#include <wiringPi.h>
#endif

#include "utils/terra_pin.h"
#include "led/terra_led.h"

BOOL terra_init()
{
#ifdef WIRINGPI
	if (wiringPiSetup()  == -1)
		return FALSE;
#else
	terra_log_info("[terra_init] wiringPi disabled!\n");
#endif
	terra_pin_set_output(LED_PIN_ERR);

	if (!terra_lock_init())
	{
		terra_log_error("failed to initialize terra lock\n");
		terra_led_set(LED_PIN_ERR, TRUE);
		return FALSE;
	}
	printf("lock init success\n");
	return TRUE;
}
