#include "terra.h"

BOOL terra_init(char const * const conf_path, BOOL const daemon)
{
	if (!terra_runtime_init(conf_path, daemon))
	{
		terra_log_error("[terra_init] failed to initialize runtime\n");
		return FALSE;
	}

//TODO: DEBUG one lvl deeper
#ifndef DEBUG
	if (pi_2_mmio_init() != MMIO_SUCCESS)
	{
		terra_log_error("[terra_init] failed to initialize mmio\n");
		return FALSE;
	}

	if (!terra_lock_init())
	{
		terra_log_error("[terra_init] failed to initialize terra lock\n");
		return FALSE;
	}
#endif

	return TRUE;
}
