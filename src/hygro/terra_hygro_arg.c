#include "terra_hygro.h"

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