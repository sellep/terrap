#include "terra.h"

#ifdef WIRINGPI
#include <wiringPi.h>
#endif

#include "conf/terra_conf.h"
#include "led/terra_led.h"

void terra_heart_beat(terra_conf const * const conf)
{
	const terra_led_cmd heart_on = LED_ON | LED_HEART;
	const terra_led_cmd heart_off = LED_OFF | LED_HEART;

#ifdef WIRINGPI
	terra_led_set(conf->led_pin_heart, heart_on);
	delay(conf->heart_dur);
	terra_led_set(conf->led_pin_heart, heart_off);
#endif
}
