#include "terra_hygro.h"

BOOL terra_hygro_read(terra_hygro_res * const res, terra_conf const * const conf)
{
	terra_lock(TERRA_HYGRO | TERRA_LOCK);
	//do something here
	terra_lock(TERRA_HYGRO | TERRA_RELEASE);
	return TRUE;
}