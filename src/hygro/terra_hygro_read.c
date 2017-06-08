#include "terra_hygro.h"

BOOL terra_hygro_read(terra_hygro_res * const res, terra_conf const * const conf)
{
#ifdef WPI_ENABLED
	LOCK();

	//sync
	digitalWrite(conf->hygro_pin_out, 1);
	delay(10);
	digitalWrite(conf->hygro_pin_out, 0);
	delay(18);

	

	UNLOCK();
#endif

	return TRUE;
}