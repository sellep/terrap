#include "terra.h"

#include "conf/terra_conf.h"
#include "utils/terra_time.h"

static int terra_clock_mode[TERRA_CONF_MAX_SCHED_CLOCKS];

#define SOCK_CLOCK_NOT_ON(i)(terra_clock_mode[i] != SOCK_ON)
#define SOCK_CLOCK_NOT_OFF(i)(terra_clock_mode[i] != SOCK_OFF)
#define SOCK_CLOCK_SET_ON(i)(terra_clock_mode[i] = SOCK_ON)
#define SOCK_CLOCK_SET_OFF(i)(terra_clock_mode[i] = SOCK_OFF)

void terrad_run_clock_init(terra_conf const * const conf)
{
	ssize_t i;

	for (i = 0; i < conf->sched_clocks_len; i++)
	{
		terra_clock_mode[i] = SOCK_UNKNOWN;
	}
}

void terrad_run_clock(terra_sched_clock const * const clock, ssize_t const i, terra_time const * const sys_time)
{
	size_t diff;

	if (!clock->sched.enabled)
		return;

	diff = terra_time_diff(&sys_time, &clock->start);
	if (diff == 0 && SOCK_CLOCK_NOT_ON(i))
	{
		SOCK_CLOCK_SET_ON(i);
		terra_log_info("switch clock %s to on\n", clock->sched.name);
	}

	diff = terra_time_diff(sys_time, &clock->end);
	if (diff == 0 && SOCK_CLOCK_NOT_OFF(i))
	{
		SOCK_CLOCK_SET_OFF(i);
		terra_log_info("switch clock %s to off\n", clock->sched.name);
	}
}