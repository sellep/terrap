#include "terra_time.h"

#ifdef _POSIX_C_SOURCE >= 199309L
	#include <time.h>
#else
	#include <unistd.h>
#endif

void terra_time_sleep(size_t const ms)
{
#ifdef _POSIX_C_SOURCE >= 199309L
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	nanosleep(&ts, NULL);
#else
	usleep(ms * 1000);
#endif
}