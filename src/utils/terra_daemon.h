#ifndef __P_TERRA_UTILS_DAEMON_H
#define __P_TERRA_UTILS_DAEMON_H

#include "../terra_defs.h"

#define ARG_MODE_START "start"
#define ARG_MODE_STOP "stop"

extern BOOL terra_daemon_pid();
extern void terra_daemon_kill();
extern void terra_daemonize();

static inline void terra_daemon_start()
{
	if (fork() == 0)
	{
		execl("/etc/init.d/terra", "/etc/init.d/terra", "start", (char*) NULL);
	}
}

static inline void terra_daemon_stop()
{
	if (fork() == 0)
	{
		execl("/etc/init.d/terra", "/etc/init.d/terra", "stop", (char*) NULL);
	}
}

static inline BOOL terra_daemon_run(int const argc, char * * const char)
{
	if (argc == 2)
	{
		terra_daemonize();
	}
	else if (strcmp(argv[2], ARG_MODE_START) == 0)
	{
		terra_daemon_start();
	}
	else if (strcmp(argv[2], ARG_MODE_STOP) == 0)
	{
		terra_daemon_stop();
	}
	else
	{
		terra_log_error("[terra_daemon_run] invalid mode\n");
		return FALSE;
	}

	return TRUE;
}

#endif