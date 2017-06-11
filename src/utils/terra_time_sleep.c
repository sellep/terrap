#include "terra_time.h"

#include <sys/time.h>
#include <time.h>
#include <errno.h>

void busy_wait_milliseconds(size_t const ms)
{
	struct timeval deltatime;
	struct timeval walltime;
	struct timeval endtime;

	deltatime.tv_sec = ms / 1000;
	deltatime.tv_usec = (ms % 1000) * 1000;

	gettimeofday(&walltime, NULL);

	timeradd(&walltime, &deltatime, &endtime);

	while (timercmp(&walltime, &endtime, <))
	{
		gettimeofday(&walltime, NULL);
	}
}

void sleep_milliseconds(size_t const ms)
{
	struct timespec sleep;

	sleep.tv_sec = ms / 1000;
	sleep.tv_nsec = (ms % 1000) * 1000000L;

	while (clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep, &sleep) && errno == EINTR);
}

void busy_wait_microseconds(size_t const ms)
{
	struct timeval tNow, tLong, tEnd;

	gettimeofday (&tNow, NULL) ;
	tLong.tv_sec  = ms / 1000000 ;
	tLong.tv_usec = ms % 1000000 ;
	timeradd (&tNow, &tLong, &tEnd) ;

	while (timercmp (&tNow, &tEnd, <)) gettimeofday (&tNow, NULL);
}

void sleep_microseconds(size_t const ms)
{
	struct timespec sleeper;

	if (ms == 0)
		return;

	if (ms  < 100)
	{
		busy_wait_microseconds(ms);
		return;
	}
	
	size_t uSecs = ms % 1000000;
	size_t wSecs = ms / 1000000;

	sleeper.tv_sec  = wSecs;
	sleeper.tv_nsec = (long)(uSecs * 1000L);
	nanosleep(&sleeper, NULL);
}