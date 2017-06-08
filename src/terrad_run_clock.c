#include "terra.h"

#include "conf/terra_conf.h"
#include "utils/terra_time.h"

static int _terra_clock_mode[TERRA_CONF_MAX_SCHED_CLOCKS];

#define SWITCH_NOT_ON(i)(_terra_clock_mode[i] != SWITCH_ON)
#define SWITCH_NOT_OFF(i)(_terra_clock_mode[i] != SWITCH_OFF)
#define SWITCH_SET_ON(i)(_terra_clock_mode[i] = SWITCH_ON)
#define SWITCH_SET_OFF(i)(_terra_clock_mode[i] = SWITCH_OFF)

#define TIME_BETWEEN(c, a, b)(terra_time_cmp(a, c) == TIME_BELOW && terra_time_cmp(b, c) == TIME_ABOVE)

void terrad_run_clock_init(terra_conf const * const conf)
{
	ssize_t i;

	for (i = 0; i < conf->sched_clocks_len; i++)
	{
		_terra_clock_mode[i] = SWITCH_UNKNOWN;
	}
}

void terrad_run_clock(terra_sched_clock const * const clock, ssize_t const i, terra_time const * const sys_time)
{
	size_t diff;

	if (!clock->sched.enabled)
		return;

	diff = terra_time_diff(sys_time, &clock->start);
	if (SWITCH_NOT_ON(i) && (diff == 0 || TIME_BETWEEN(sys_time, &clock->start, &clock->end)))
	{
		SWITCH_SET_ON(i);
		terra_log_info("switch clock %s to on (diff: %us)\n", clock->sched.name, diff);
	}

	diff = terra_time_diff(sys_time, &clock->end);
	if (SWITCH_NOT_OFF(i) && (diff == 0 || terra_time_cmp(sys_time, &clock->end) == TIME_ABOVE))
	{
		SWITCH_SET_OFF(i);
		terra_log_info("switch clock %s to off (diff: %us)\n", clock->sched.name, diff);
	}
}