#include "terra_hygro.h"

#include "../pi_2_dht_read.h"

#define HYGRO_READ_MODE "read"
#define HYGRO_WRITE_MODE "write"

BOOL terra_hygro_arg(terra_hygro_cmd * const cmd, ssize_t const argc, char const * const * const argv)
{
	if (argc == 2)
	{
		terra_log_error("[terra_hygro_arg] insufficient args\n");
		return FALSE;
	}

	if (strcmp(argv[2], HYGRO_READ_MODE) == 0)
	{
		cmd->read = TRUE;
	}
	else if (strcmp(argv[2], HYGRO_WRITE_MODE) == 0)
	{
		if (argc != 5)
		{
			terra_log_error("[terra_hygro_arg] insufficient args for write\n");
			return FALSE;
		}

		cmd->humi = atof(argv[3]);
		cmd->temp = atof(argv[4]);

		cmd->read = FALSE;
	}
	else
	{
		terra_log_error("[terra_hygro_arg] unknown mode %s\n", argv[2]);
		return FALSE;
	}

	return TRUE;
}

BOOL terra_hygro_run(terra_conf const * const conf, terra_hygro_cmd const * const cmd)
{
	terra_time now;
	int i;
	int status;

	if (cmd->read)
	{
		float t;
		float h;

		/*
			obsolete		
		if (!terra_hygro_read_rep(&conf, &h, &t))
		{
			terra_log_error("[terra_hygro_run] failed to read hygro\n");
			return FALSE;
		}*/

		for (i = 0; i <= 10; i++)
		{
			status = pi_2_dht_read(DHT22, conf->hygro_pin_io, &h, &t);
			if (status == DHT_SUCCESS)
				break;

			if (status == DHT_ERROR_TIMEOUT)
				continue;

			if (status == DHT_ERROR_CHECKSUM)
				continue;

			terra_log_error("[terra_hygro_run] failed to read dht (%i)\n", status);
			return FALSE;
		}

		if (i > 10)
		{
			terra_log_error("[terra_hygro_run] failed to read dht repeated\n");
			return FALSE;
		}

		terra_log_info("[terra_hygro_run] temperature: %.2f, humidity: %.2f\n", t, h);
	}
	else
	{
		terra_time_now(&now);

		if (!terra_hygro_write(conf, &now, cmd->humi, cmd->temp))
		{
			terra_log_error("[terra_hygro_run] failed to write data\n");
			return FALSE;
		}
	}

	return TRUE;
}
