#include "terra_schedule.h"

#define ARG_SCHEDULE_LOGFILE "logfile"

void terra_schedule_arg(int const argc, char const * const * const argv)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("arg%i: %s\n", i, argv[i]);
	}
	return;
	if (argc >= 3 && strcmp(argv[2], ARG_SCHEDULE_LOGFILE) == 0)
	{
		terra_log_use_file();
	}
}