#include "terra_runtime.h"

BOOL terra_runtime_init(char const * const conf_path)
{
#ifndef DEBUG
	if (pi_2_mmio_init() != MMIO_SUCCESS)
	{
		terra_log_error("[terra_runtime_init] failed to initialize mmio\n");
		return FALSE;
	}
#endif

	if (!terra_lock_init())
	{
		terra_log_error("[terra_runtime_init] failed to initialize lock\n");
		return FALSE;
	}

	if (!terra_conf_read(&CONF_GLOBAL, conf_path))
	{
		terra_log_error("[terra_runtime_init] failed to read config file\n");
		return FALSE;
	}

	runtime.hygro_err = 0;

	terra_pin_set_out(CONF_SWITCH.pin);
	terra_pin_set_out(CONF_HEART.pin);
	terra_pin_set_out(CONF_GLOBAL.pin_alert);

	runtime.tick = 0;
	terra_time_now(&runtime.now);

	return TRUE;
}
