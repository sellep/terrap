#include "terra.h"

#include "conf/terra_conf.h"
#include "utils/terra_time.h"

static int _terra_clock_mode[TERRA_CONF_MAX_SCHED_CLOCKS];
static size_t _terra_clock_ldiff_start[TERRA_CONF_MAX_SCHED_CLOCKS];
static size_t _terra_clock_ldiff_end[TERRA_CONF_MAX_SCHED_CLOCKS];

#define SOCK_CLOCK_NOT_ON(i)(_terra_clock_mode[i] != SOCK_ON)
#define SOCK_CLOCK_NOT_OFF(i)(_terra_clock_mode[i] != SOCK_OFF)
#define SOCK_CLOCK_SET_ON(i)(_terra_clock_mode[i] = SOCK_ON)
#define SOCK_CLOCK_SET_OFF(i)(_terra_clock_mode[i] = SOCK_OFF)

#define LAST_START_DIFF_SMALLER(i,diff)(_terra_clock_ldiff_start[i] < diff)
#define LAST_END_DIFF_SMALLER(i,diff)(_terra_clock_ldiff_end[i] < diff)

void terrad_run_clock_init(terra_conf const * const conf)
{
	ssize_t i;

	for (i = 0; i < conf->sched_clocks_len; i++)
	{
		_terra_clock_mode[i] = SOCK_UNKNOWN;
		_terra_clock_ldiff_start[i] = (size_t)-1;
		_terra_clock_ldiff_end[i] = (size_t)-1;
	}
}

void terrad_run_clock(terra_sched_clock const * const clock, ssize_t const i, terra_time const * const sys_time)
{
	size_t diff;

	if (!clock->sched.enabled)
		return;

	diff = terra_time_diff(sys_time, &clock->start);
	if (SOCK_CLOCK_NOT_ON(i) && (diff == 0 || LAST_START_DIFF_SMALLER(i, diff)))
	{
		SOCK_CLOCK_SET_ON(i);
		terra_log_info("switch clock %s to on\n", clock->sched.name);
	}

	_terra_clock_ldiff_start[i] = diff;

	diff = terra_time_diff(sys_time, &clock->end);
	if (SOCK_CLOCK_NOT_OFF(i) && (diff == 0 || LAST_END_DIFF_SMALLER(i, diff)))
	{
		SOCK_CLOCK_SET_OFF(i);
		terra_log_info("switch clock %s to off\n", clock->sched.name);
	}

	_terra_clock_ldiff_end[i] = diff;
}