#ifndef __P_TERRA_LED_H
#define __P_TERRA_LED_H

#include "../terra.h"

#define LED_PIN_ERR 0

enum led_cmds
{
	LED_NONE = 0,
	LED_ERR = 2,
	LED_WARN = 4,
	LED_ON = 8,
	LED_OFF = 16
};

typedef int terra_led_cmd;

void terra_led_set(ssize_t const, terra_led_cmd const);
terra_led_cmd terra_led_arg(int const argc, char const * const * const argv);
BOOL terra_led_set_from_cmd(terra_conf const * const, terra_led_cmd const);

#endif