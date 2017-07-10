#ifndef __P_TERRA_UTILS_LOG_H
#define __P_TERRA_UTILS_LOG_H

#include <stdarg.h>
#include <stdio.h>

#ifdef SYSLOG_ENABLED
	#include <syslog.h>
	#define TERRA_LOG_ID "terrad"
#endif

extern void terra_log_info(char const * const, ...);
extern void terra_log_error(char const * const, ...);

#endif
