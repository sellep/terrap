#include "terra_log.h"

void terra_log_info(char const * const msg, va_list ap)
{
#if SYSLOG_ENABLED
	openlog(TERRA_LOG_ID, LOG_PID, LOG_DAEMON);
	syslog(LOG_INFO, msg, ap);
	closelog();
#else
	printf(msg, ap);
#endif
}

void terra_log_error(char const * const msg, va_list ap)
{
#if SYSLOG_ENABLED
	openlog(TERRA_LOG_ID, LOG_PID, LOG_DAEMON);
	syslog(LOG_ERR, msg, ap);
	closelog();
#else
	fprintf(stderr, msg, ap);
#endif
}