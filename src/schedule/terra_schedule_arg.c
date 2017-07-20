#include "terra_schedule.h"

#define ARG_SCHEDULE_LOGFILE "logfile"

void terra_schedule_arg(int const argc, char const * const * const argv)
{
	if (argc >= 3 && strcmp(argv[2], ARG_SCHEDULE_LOGFILE) == 0)
	{
		printf("use log file\n");
		//terra_log_use_file();
	}
}