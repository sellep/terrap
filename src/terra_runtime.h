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
#include "utils/terra_daemon.h"
#include "utils/terra_group.h"
#include "switch/terra_switch.h"
#include "led/terra_led.h"
#include "hygro/terra_hygro.h"

typedef struct
{
	terra_conf conf;
	BOOL terminate;
	BOOL reload;

	uint64_t tick;
	terra_time now;
	terra_time hygro_last;
	size_t hygro_err;

	terra_switch_mode switch_modes[3];

	float temp;
	float humi;

	terra_group group;
} terra_runtime;

terra_runtime runtime;

extern BOOL terra_runtime_init(char const * const);

#define RUNTIME_SET_TERMINATE() runtime.terminate = TRUE
#define RUNTIME_SET_RELOAD() runtime.reload = TRUE
#define RUNTIME_TERMINATE() runtime.terminate
#define RUNTIME_RELOAD() runtime.reload
#define RUNTIME_BREAK() (RUNTIME_TERMINATE() || RUNTIME_RELOAD())
#define RUNTIME_RESET_RELOAD() runtime.reload = FALSE

#define CONF_GLOBAL runtime.conf
#define CONF_LED runtime.conf.ld
#define CONF_SWITCH runtime.conf.sw
#define CONF_HYGRO runtime.conf.hy

#define CONF_FREE() terra_conf_free(&runtime.conf)
#define CONF_LOAD() terra_conf_read(&CONF_GLOBAL, CONF_PATH)

#define RUNTIME_GROUP runtime.group

#define NOW runtime.now

#define RUNTIME_SWITCH_GET(s) terra_runtime_switch_get(s)
#define RUNTIME_SWITCH_SET_ON(s, sender) terra_runtime_switch_set_on(s, sender)
#define RUNTIME_SWITCH_SET_OFF(s, sender) terra_runtime_switch_set_off(s, sender)
#define RUNTIME_SWITCH_NOT_ON(s) RUNTIME_SWITCH_GET(s) != SWITCH_ON
#define RUNTIME_SWITCH_NOT_OFF(s) RUNTIME_SWITCH_GET(s) != SWITCH_OFF
#define RUNTIME_SWITCH_ON(s) RUNTIME_SWITCH_GET(s) == SWITCH_ON
#define RUNTIME_SWITCH_OFF(s) RUNTIME_SWITCH_GET(s) == SWITCH_OFF

#define RUNTIME_TEMP runtime.temp
#define RUNTIME_HUMI runtime.humi

static inline terra_switch_mode terra_runtime_switch_get(char const sock)
{
	if (sock == 'a')
		return runtime.switch_modes[0];

	if (sock == 'b')
		return runtime.switch_modes[1];

	return runtime.switch_modes[2];
}

static inline void terra_runtime_switch_set_on(char const socket, int const channel, char const * const sender)
{
	terra_switch_set_on(&CONF_SWITCH, socket, channel, sender);

	if (socket == 'a')
	{
		runtime.switch_modes[0] = SWITCH_ON;
	}
	else if (socket == 'b')
	{
		runtime.switch_modes[1] = SWITCH_ON;
	}
	else
	{
		runtime.switch_modes[2] = SWITCH_ON;
	}
}

static inline void terra_runtime_switch_set_off(char const socket, int const channel, char const * const sender)
{
	terra_switch_set_off(&CONF_SWITCH, socket, channel, sender);

	if (socket == 'a')
	{
		runtime.switch_modes[0] = SWITCH_OFF;
	}
	else if (socket == 'b')
	{
		runtime.switch_modes[1] = SWITCH_OFF;
	}
	else
	{
		runtime.switch_modes[2] = SWITCH_OFF;
	}
}

static inline void terra_pin_set_out(int const pin)
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

static inline void terra_exit(int const code)
{
	terra_daemon_kill();
	terra_led_set(CONF_LED.err_pin, LED_ON);
	exit(code);
}

#endif
