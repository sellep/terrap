#include "terra_schedule.h"

#define ARG_SCHEDULE_LOGFILE "logfile"
#define ARG_SCHEDULE_PIDFILE "pidfile"

void terra_schedule_arg(int const argc, char * * const argv)
{
	if (argc >= 3 && strcmp(argv[2], ARG_SCHEDULE_LOGFILE) == 0)
	{
		terra_log_use_file();
	}

	if (argc >= 4 && strcmp(argv[3], ARG_SCHEDULE_PIDFILE) == 0)
	{
		terra_daemon_pid();
	}
}