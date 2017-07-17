#include "terra_log.h"

#include <time.h>

#define MAX_BUF_LEN 1024
#define MESSAGE_FORMAT "terra %02zu:%02zu %02zu.%02zu.%zu: %s"

static char buf[MAX_BUF_LEN];

static inline struct tm log_time()
{
	time_t t = time(NULL);
	return *localtime(&t);
}

void terra_log_info(char const * const msg, ...)
{
	va_list args;
	va_start(args, msg);
	vsnprintf(buf, MAX_BUF_LEN - 1, msg, args);

	struct tm ts = log_time();

#ifdef SYSLOG_ENABLED
	openlog(TERRA_LOG_ID, LOG_PID, LOG_DAEMON);
	syslog(LOG_INFO, MESSAGE_FORMAT, ts.tm_hour, ts.tm_min, ts.tm_mday, ts.tm_mon + 1, ts.tm_year + 1900, buf);
	closelog();
#else
	printf(MESSAGE_FORMAT, ts.tm_hour, ts.tm_min, ts.tm_mday, ts.tm_mon + 1, ts.tm_year + 1900, buf);
#endif

	va_end(args);
}

void terra_log_error(char const * const msg, ...)
{
	va_list args;
	va_start(args, msg);
	vsnprintf(buf, MAX_BUF_LEN - 1, msg, args);

	struct tm ts = log_time();

#ifdef SYSLOG_ENABLED
	openlog(TERRA_LOG_ID, LOG_PID, LOG_DAEMON);
	syslog(LOG_ERR, MESSAGE_FORMAT, ts.tm_hour, ts.tm_min, ts.tm_mday, ts.tm_mon + 1, ts.tm_year + 1900, buf);
	closelog();
#else
	fprintf(stderr, MESSAGE_FORMAT, ts.tm_hour, ts.tm_min, ts.tm_mday, ts.tm_mon + 1, ts.tm_year + 1900, buf);
#endif

	va_end(args);
}
