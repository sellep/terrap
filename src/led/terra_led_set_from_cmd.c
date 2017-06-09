#include "terra_led.h"

BOOL terra_led_set_from_cmd(terra_conf const * const conf, terra_led_cmd const cmd)
{
	if (cmd == LED_NONE)
		return TRUE;

	if (cmd & LED_ERR)
	{
		terra_led_set(LED_PIN_ERR, cmd);
		return TRUE;
	}
	else if (cmd & LED_WARN)
	{
		terra_led_set(conf->led_pin_warn, cmd);
		return TRUE;
	}

	terra_log_error("[terra_led_set_from_cmd] missing led target\n");
	return FALSE;
}