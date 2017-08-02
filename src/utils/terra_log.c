#include "terra_log.h"

#include <time.h>

#define MAX_BUF_LEN 1024
#define INFO_MESSAGE_FORMAT "%02zu:%02zu:%02zu %02zu.%02zu.%zu [INFO] %s"
#define ERROR_MESSAGE_FORMAT "%02zu:%02zu:%02zu %02zu.%02zu.%zu [ERROR] %s"

#define FILE_PATH "/var/log/terra"

static char buf[MAX_BUF_LEN];
static BOOL use_log = FALSE;

static inline struct tm log_time()
{
	time_t t = time(NULL);
	return *localtime(&t);
}

void terra_log_use_file()
{
	use_log = TRUE;
}

void terra_log_info(char const * const msg, ...)
{
	FILE *f;

	va_list args;
	va_start(args, msg);
	vsnprintf(buf, MAX_BUF_LEN - 1, msg, args);

	struct tm ts = log_time();

	if (use_log)
	{
		f = fopen(FILE_PATH, "a");
		if (!f)
		{
			sprintf(stderr, "[terra_log_info] failed to open file\n");
			goto end;
		}

		fprintf(f, INFO_MESSAGE_FORMAT, ts.tm_hour, ts.tm_min, ts.tm_sec, ts.tm_mday, ts.tm_mon + 1, ts.tm_year + 1900, buf);

		fclose(f);
	}
	else
	{
		printf(INFO_MESSAGE_FORMAT, ts.tm_hour, ts.tm_min, ts.tm_sec, ts.tm_mday, ts.tm_mon + 1, ts.tm_year + 1900, buf);
	}

end:
	va_end(args);
}

void terra_log_error(char const * const msg, ...)
{
	FILE *f;

	va_list args;
	va_start(args, msg);
	vsnprintf(buf, MAX_BUF_LEN - 1, msg, args);

	struct tm ts = log_time();

	if (use_log)
	{
		f = fopen(FILE_PATH, "a");
		if (!f)
		{
			sprintf(stderr, "[terra_log_error] failed to open file\n");
			goto end;
		}

		fprintf(f, ERROR_MESSAGE_FORMAT, ts.tm_hour, ts.tm_min, ts.tm_mday, ts.tm_mon + 1, ts.tm_year + 1900, buf);

		fclose(f);
	}
	else
	{
		fprintf(stderr, ERROR_MESSAGE_FORMAT, ts.tm_hour, ts.tm_min, ts.tm_mday, ts.tm_mon + 1, ts.tm_year + 1900, buf);
	}

end:
	va_end(args);
}
