#include "terra_log.h"

void terra_log_info(char const * const msg, va_list ap)
{
	openlog(TERRA_LOG_ID, LOG_PID, LOG_DAEMON);
	syslog(LOG_INFO, msg, ap);
	closelog();
}