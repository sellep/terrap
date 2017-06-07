#ifndef __P_TERRA_UTILS_LOG_H
#define __P_TERRA_UTILS_LOG_H

#include <syslog.h>
#include <stdarg.h>

#define TERRA_LOG_ID "terrad"

void terra_log_info(char const * const, va_list ap);

#endif