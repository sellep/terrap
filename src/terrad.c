#include "terra.h"

#include "utils/terra_log.h"

int main(int argc, char **argv)
{
	terra_conf conf;

	terra_log_info("terra daemon started\n");

	if (argc != 2)
	{
		terra_log_error("insufficient argument count\n");
		return 1;
	}

	terra_log_info("config file path: %s\n", argv[1]);

	if (!terra_conf_read(&conf))
	{
		terra_log_error("failed to read config file\n");
		return 1;
	}

	return 0;
}