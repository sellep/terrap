#include "terra.h"

#include "conf/terra_conf.h"
#include "led/terra_led.h"
#include "utils/terra_time.h"

void terra_heart_beat(terra_conf const * const conf)
{
	const terra_led_cmd heart_on = LED_ON | LED_HEART;
	const terra_led_cmd heart_off = LED_OFF | LED_HEART;

	terra_led_set(conf->led_pin_heart, heart_on);
	sleep_milliseconds(conf->heart_dur);
	terra_led_set(conf->led_pin_heart, heart_off);
}
