#include "terra_hygro.h"

enum hygro_statuses
{
	HYGRO_OK,
	HYGRO_DHT_FAILED,
	HYGRO_REPEATED_FAILED
};

typedef int hygro_status;

static inline hygro_status hygro_read(float * const h, float * const t, terra_conf_hygro const * const conf)
{
#ifdef DEBUG
	return HYGRO_OK;
#else
	ssize_t i;
	int status;

	for (i = 0; i <= conf->repeats; i++)
	{
		status = pi_2_dht_read(DHT22, conf->pin, h, t);
		if (LIKELY(status == DHT_SUCCESS))
		{
			if (UNLIKELY(h[0] > 100 || t[0] > 100 || h[0] == 0 || t[0] == 0))
				continue;

			return HYGRO_OK;
		}

		if (status == DHT_ERROR_TIMEOUT)
			continue;

		if (status == DHT_ERROR_CHECKSUM)
			continue;

		return HYGRO_DHT_FAILED;
	}

	return HYGRO_REPEATED_FAILED;
#endif
}


BOOL terra_hygro_read(float * const h, float * const t, terra_conf_hygro const * const conf, size_t * const err_count)
{
	hygro_status status = hygro_read(h, t, conf);
	if (LIKELY(status == HYGRO_OK))
	{
		err_count[0] = 0;
		return TRUE;
	}

	err_count[0]++;

	if (LIKELY(status == HYGRO_REPEATED_FAILED))
	{
		terra_log_error("[terra_hygro_run] failed to read dht repeated (%zu)\n", err_count[0]);
	}
	else
	{
		terra_log_error("[terra_hygro_run] failed to read dht (%zu)\n", err_count[0]);
	}

	return FALSE;
}