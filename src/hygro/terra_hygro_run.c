#include "terra_hygro.h"

BOOL terra_hygro_run(float * const h, float * const t)
{
	ssize_t i;
	int status;

	for (i = 0; i <= CONF_HYGRO.repeats; i++)
	{
		status = pi_2_dht_read(DHT22, CONF_HYGRO.pin, h, t);
		if (status == DHT_SUCCESS)
		{
			if (h > 100 || t > 100)
				continue;

			return TRUE;
		}

		if (UNLIKELY(status == DHT_ERROR_TIMEOUT))
			continue;

		if (status == DHT_ERROR_CHECKSUM)
			continue;

		terra_log_error("[terra_hygro_run] failed to read dht (%i)\n", status);
		return FALSE;
	}

	terra_log_error("[terra_hygro_run] failed to read dht repeated\n");
	return FALSE;
}
