#ifndef __P_TERRA_UTILS_DAEMON_H
#define __P_TERRA_UTILS_DAEMON_H

#include "../terra_defs.h"

extern BOOL terra_daemon_pid();
extern void terra_daemon_kill();
extern void terra_daemonize();

#endif