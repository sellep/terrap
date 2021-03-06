#include "terra_runtime.h"

BOOL terra_runtime_init(char const * const conf_path)
{
#ifndef DEBUG
	printf("b\n");

	if (pi_2_mmio_init() != MMIO_SUCCESS)
	{
		printf("b.0\n");
		
		terra_log_error("[terra_runtime_init] failed to initialize mmio\n");
		return FALSE;
	}
#endif

printf("b.1\n");

	if (!terra_lock_init())
	{
		printf("b.2\n");
		terra_log_error("[terra_runtime_init] failed to initialize lock\n");
		return FALSE;
	}

	printf("b.3\n");
	
	if (!terra_conf_parse(&CONF_GLOBAL, conf_path))
	{
		printf("b.4\n");
		terra_log_error("[terra_runtime_init] failed to read config file\n");
		return FALSE;
	}
	
	printf("b.5\n");
	
	if (!terra_switch_init(&runtime.switch_modes, &CONF_SWITCH))
	{
		terra_log_error("[terra_runtime_init] failed to init sockets\n");
		return FALSE;
	}
	
	printf("b.6\n");

	runtime.hygro_err = 0;

	terra_pin_set_out(CONF_SWITCH.pin);
	terra_pin_set_out(CONF_LED.heart_pin);
	terra_pin_set_out(CONF_LED.err_pin);

	runtime.tick = 0;
	terra_time_now(&runtime.now);

	return TRUE;
}
