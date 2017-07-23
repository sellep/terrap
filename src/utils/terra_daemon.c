#include "terra_daemon.h"

#define PID_FILE "/var/run/terra.pid"

static int pid_h = 0;

BOOL terra_daemon_pid()
{
	char str[10];

	pid_h = open(PID_FILE, O_RDWR | O_CREAT, OWNER_READ_WRITE);
	if (pid_h == -1)
	{
		terra_log_error("[terra_daemon_pid] failed to create pid file\n");
		return FALSE;
	}

	if (lockf(pid_h, F_TLOCK, 0) == -1)
	{
		terra_log_error("[terra_daemon_pid] failed to lock pid file\n");
		return FALSE;
	}

	sprintf(str,"%d\n",getpid());
	write(pid_h, str, strlen(str));
	return TRUE;
}

void terra_daemon_kill()
{
	if (pid_h)
	{
		close(pid_h);
	}
}

void terra_daemonize()
{
	if (fork() == 0)
	{
		if (execl("/usr/local/bin/terra", "/usr/local/bin/terra", "schedule", "logfile", "pidfile", (char*) NULL) == -1)
		{
			terra_log_error("[terra] failed to execl (%s)\n", strerror(errno));
		}
	}
}
