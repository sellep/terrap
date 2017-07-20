#include "terra_signal.h"

#include "../terra_runtime.h"

#include <errno.h>

void terra_signal_handler(int const signum)
{
	if (signum == SIGHUP)
	{
		terra_log_info("[terra_signal_handler] received signal SIGHUP(%i)\n", signum);
		RUNTIME_SET_RELOAD();
	}
	else
	{
		terra_log_info("[terra_signal_handler] received signal %s(%i)\n", signum == 2 ? "SIGINT" : "SIGTERM", signum);
		RUNTIME_SET_TERMINATE();
	}
}

void terra_signal_sighup()
{
	char buf[10];
	int num, pid;

	FILE *f = fopen("/var/run/terra", "r");
	if (!f)
	{
		terra_log_error("[terra_signal_sighup] daemon not running\n");
		return;
	}

	num = fread(buf, 1, sizeof buf, f);

	fclose(f);

	pid = atoi(buf);
	if (!pid)
	{
		terra_log_error("[terra_signal_sighup] failed to parse pid\n");
		return;
	}

	if (kill(pid, SIGHUP) == -1)
	{
		terra_log_error("[terra_signal_sighup] failed to send signal (%s)\n", strerror(errno));
	}
}