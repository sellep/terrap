#ifndef __P_TERRA_SIGNAL_H
#define __P_TERRA_SIGNAL_H

#include <stdint.h>
#include <signal.h>

#include "../terra_defs.h"
#include "terra_log.h"

extern void terra_signal_handler(int const);
extern void terra_signal_sighup();

static inline BOOL terra_signal_reg()
{
	if (signal(SIGINT, terra_signal_handler) == SIG_ERR)
	{
		terra_log_error("[terra_signal_reg] unable to register signal handler for SIGINT\n");
		return FALSE;
	}

	if (signal(SIGTERM, terra_signal_handler) == SIG_ERR)
	{
		terra_log_error("[terra_signal_reg] unable to register signal handler for SIGTERM\n");
		return FALSE;
	}

	if (signal(SIGHUP, terra_signal_handler) == SIG_ERR)
	{
		terra_log_error("[terra_signal_reg] unable to register signal handler for SIGHUP\n");
		return FALSE;
	}

	return TRUE;
}

#endif