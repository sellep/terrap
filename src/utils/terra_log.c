#include "terra_log.h"

#ifdef SYSLOG_ENABLED
	#include <syslog.h>
	#define TERRA_LOG_ID "terrad"
#else
	#include <stdio.h>
#endif

#define MAX_BUF_LEN 1024

void terra_log_info(char const * const msg, ...)
{
	char buf[MAX_BUF_LEN];
	va_list args;
	va_start(args, msg);
	vsnprintf(buf, MAX_BUF_LEN - 1, msg, args);

#ifdef SYSLOG_ENABLED
	openlog(TERRA_LOG_ID, LOG_PID, LOG_DAEMON);
	syslog(LOG_INFO, buf);
	closelog();
#else
	printf(buf);
#endif

	va_end(args);
}

void terra_log_error(char const * const msg, ...)
{
	char buf[MAX_BUF_LEN];
	va_list args;
	va_start(args, msg);
	vsnprintf(buf, MAX_BUF_LEN - 1, msg, args);

#ifdef SYSLOG_ENABLED
	openlog(TERRA_LOG_ID, LOG_PID, LOG_DAEMON);
	syslog(LOG_ERR, buf);
	closelog();
#else
	fprintf(stderr, buf);
#endif

	va_end(args);
}
