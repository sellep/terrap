#include "terra_hygro.h"

BOOL terra_hygro_run(terra_hygro_cmd const * const cmd)
{
	int i;
	int status;

	float t;
	float h;

	for (i = 0; i <= conf.hygro_rep; i++)
	{
		status = pi_2_dht_read(DHT22, conf.hygro_pin_io, &h, &t);
		if (status == DHT_SUCCESS)
			break;

		if (status == DHT_ERROR_TIMEOUT)
			continue;

		if (status == DHT_ERROR_CHECKSUM)
			continue;

		terra_log_error("[terra_hygro_run] failed to read dht (%i)\n", status);
		return FALSE;
	}

	if (i > conf.hygro_rep)
	{
		terra_log_error("[terra_hygro_run] failed to read dht repeated\n");
		return FALSE;
	}

	//TODO: move out
	terra_log_info("[terra_hygro_run] temperature: %.2f, humidity: %.2f\n", t, h);

	if (cmd->read)
		return TRUE;

	if (!terra_hygro_write(cmd->humi, cmd->temp))
	{
		terra_log_error("[terra_hygro_run] failed to write data\n");
		return FALSE;
	}

	return TRUE;
}
