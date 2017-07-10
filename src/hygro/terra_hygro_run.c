#include "terra_hygro.h"

BOOL terra_hygro_run(float * const h, float * const t)
{
	ssize_t i;
	int status;

	for (i = 0; i <= conf.hygro_rep; i++)
	{
		status = pi_2_dht_read(DHT22, conf.hygro_pin_io, h, t);
		if (status == DHT_SUCCESS)
			break;

		if (UNLIKELY(status == DHT_ERROR_TIMEOUT))
			continue;

		if (status == DHT_ERROR_CHECKSUM)
			continue;

		terra_log_error("[terra_hygro_run] failed to read dht (%i)\n", status);
		return FALSE;
	}

	if (UNLIKELY(i > conf.hygro_rep))
	{
		terra_log_error("[terra_hygro_run] failed to read dht repeated\n");
		return FALSE;
	}

	return TRUE;
}
