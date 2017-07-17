#include "terra_hygro.h"

BOOL terra_hygro_read(float * const h, float * const t, terra_conf_hygro const * const conf)
{
#ifdef DEBUG
	terra_log_info("[terra_hygro_run] debug compilation\n");
	return TRUE;
#else
	ssize_t i;
	int status;

	for (i = 0; i <= conf->repeats; i++)
	{
		status = pi_2_dht_read(DHT22, conf->pin, h, t);
		if (status == DHT_SUCCESS)
		{
			if (h[0] > 100 || t[0] > 100 || h[0] == 0 || t[0] == 0)
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
#endif
}
