#include "terra_led.h"

#define ARG_LED_ERR "err"
#define ARG_LED_HEART "heart"

#define ARG_LED_ON "on"
#define ARG_LED_OFF "off"

terra_led_cmd terra_led_arg(int const argc, char const * const * const argv)
{
	terra_led_cmd res;

	if (argc != 4)
	{
		terra_log_error("insufficient led argument count\n");
		return res;
	}

//parse led target
	if (strcmp(argv[2], ARG_LED_ERR) == 0)
	{
		res = LED_ERR;
	}
	else if (strcmp(argv[2], ARG_LED_HEART) == 0)
	{
		res = LED_HEART;
	}
	else
	{
		terra_log_error("invalid led target argument\n");
		return LED_NONE;
	}

//parse led set
	if (strcmp(argv[3], ARG_LED_ON) == 0)
	{
		res |= LED_ON;
	}
	else if (strcmp(argv[3], ARG_LED_OFF) == 0)
	{
		res |= LED_OFF;
	}
	else
	{
		terra_log_error("invalid led set argument\n");
		return LED_NONE;
	}

	return res;
}
