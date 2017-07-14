#ifndef __P_TERRA_RUNTIME_H
#define __P_TERRA_RUNTIME_H

#include "pi_2_mmio.h"
#include "common_dht_read.h"
#include "pi_2_dht_read.h"

#include "terra_defs.h"
#include "conf/terra_conf.h"
#include "utils/terra_log.h"
#include "utils/terra_lock.h"
#include "utils/terra_time.h"
#include "switch/terra_switch.h"
#include "led/terra_led.h"

typedef struct
{
	terra_conf conf;

	uint64_t tick;
	terra_time now;
	terra_time hygro_last;
	int hygro_err;

	terra_switch_mode switch_modes[3];
} terra_runtime;

terra_runtime runtime;

extern BOOL terra_runtime_init(char const * const);

#define CONF_GLOBAL runtime.conf
#define CONF_HEART runtime.conf.he
#define CONF_SWITCH runtime.conf.sw
#define CONF_HYGRO runtime.conf.hy

#define NOW runtime.now

#define RUNTIME_SWITCH_GET(s) terra_runtime_switch_get(s)
#define RUNTIME_SWITCH_SET_ON(s) terra_runtime_switch_set_on(s)
#define RUNTIME_SWITCH_SET_OFF(s) terra_runtime_switch_set_off(s)
#define RUNTIME_SWITCH_NOT_ON(s) RUNTIME_SWITCH_GET(s) != SWITCH_ON
#define RUNTIME_SWITCH_NOT_OFF(s) RUNTIME_SWITCH_GET(s) != SWITCH_OFF
#define RUNTIME_SWITCH_ON(s) RUNTIME_SWITCH_GET(s) == SWITCH_ON
#define RUNTIME_SWITCH_OFF(s) RUNTIME_SWITCH_GET(s) == SWITCH_OFF

static inline terra_switch_mode terra_runtime_switch_get(char const sock)
{
	if (sock == 'a') return runtime.switch_modes[0];
	if (sock == 'b') return runtime.switch_modes[1];
	return runtime.switch_modes[2];
}

static inline void terra_runtime_switch_set_on(char const sock)
{
	terra_switch_set_on(&CONF_SWITCH, sock);

	if (sock == 'a') runtime.switch_modes[0] = SWITCH_ON;
	else if (sock == 'b') runtime.switch_modes[1] = SWITCH_ON;
	else runtime.switch_modes[2] = SWITCH_ON;
}

static inline void terra_runtime_switch_set_off(char const sock)
{
	terra_switch_set_off(&CONF_SWITCH, sock);

	if (sock == 'a') runtime.switch_modes[0] = SWITCH_OFF;
	else if (sock == 'b') runtime.switch_modes[1] = SWITCH_OFF;
	else runtime.switch_modes[2] = SWITCH_OFF;
}

static inline terra_pin_set_out(int const pin)
{
	LOCK();
#ifndef DEBUG
	pi_2_mmio_set_output(pin);
#endif	
	UNLOCK();
}

static inline void terra_runtime_tick()
{
	runtime.tick++;
	terra_time_now(&runtime.now);
}

/*static inline terra_scheduler* find_scheduler_by_name(char const * const name)
{
	terra_scheduler_clock *clock;
	terra_scheduler_period *period;
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		clock = SCHEDULER_CLOCK(i);

		if (strcmp(name, clock->scheduler.name) == 0)
			return &clock->scheduler;
	}

	for (i = 0; i < CONF_GLOBAL.period_len; i++)
	{
		period = SCHEDULER_PERIOD(i);

		if (strcmp(name, period->scheduler.name) == 0)
			return &period->scheduler;
	}

	terra_log_error("[find_scheduler_by_name] scheduler not found\n");
	//TODO: terra_exit(1);
}

static inline BOOL terra_scheduler_deps(void * target)
{
	terra_scheduler *sched = (terra_scheduler*) target;

	if (sched->depends == NULL)
		return TRUE;

	return find_scheduler_by_name(sched->name)->state == SWITCH_ON;
}*/

#endif
