#include "terra_schedule.h"

#define ARG_SCHEDULE_LOGFILE "logfile"

void terra_schedule_arg(int const argc, char const * const * const argv)
{
	if (argc >= 3 && strcmp(argv[2], ARG_SCHEDULE_LOGFILE) == 0)
	{
		TERRA_LOG_USE_FILE();
	}
}