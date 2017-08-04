#include "terra_switch.h"

#define STRING_UNKNOWN "UNKNOWN"
#define STRING_ON "ON"
#define STRING_OFF "OFF"
#define STRING_ERROR "ERROR"

static inline char* switch_print_get_string(terra_switch_mode const mode)
{
	if (mode == SWITCH_UNKNOWN)
		return STRING_UNKNOWN;
	if (mode == SWITCH_ON)
		return STRING_ON;
	if (mode == SWITCH_OFF)
		return STRING_OFF;
	return STRING_ERROR;
}

void terra_switch_print(terra_switch_mode const * const modes, terra_conf_switch const * const conf)
{
	ssize_t s;

	printf("switch socket a: %s\n", switch_print_get_string(modes[0]));
	printf("switch socket b: %s\n", switch_print_get_string(modes[0]));
	printf("switch socket c: %s\n", switch_print_get_string(modes[0]));
}