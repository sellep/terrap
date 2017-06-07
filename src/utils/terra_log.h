#ifndef __P_TERRA_UTILS_LOG_H
#define __P_TERRA_UTILS_LOG_H

#if SYSLOG_ENABLED
	#include <syslog.h>
	#define TERRA_LOG_ID "terrad"
#else
	#include <stdio.h>
#endif

#include <stdarg.h>

void terra_log_info(char const * const, va_list);
void terra_log_error(char const * const, va_list);

#endif