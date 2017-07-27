#include "terra_schedule.h"

#define INVERS_DEP(s) (s)->dep[0] == '!'
#define DEP_NAME(s) (INVERS_DEP(s)) ? (s)->dep + 1 : (s)->dep

terra_schedule* schedule_dep(terra_schedule const * const base)
{
	terra_schedule *dep;
	size_t i;
	char *name = DEP_NAME(base);

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		dep = SCHEDULE(SCHEDULE_GET_CLOCK(i));

		if (strcmp(name, dep->name) == 0)
			return dep;
	}

	for (i = 0; i < CONF_GLOBAL.hygro_len; i++)
	{
		dep = SCHEDULE(SCHEDULE_GET_HYGRO(i));

		if (strcmp(name, dep->name) == 0)
			return dep;
	}

	terra_log_error("[terra_schedule.dep_get] dependency %s not found\n", name);
	terra_exit(1);
	return NULL;
}

BOOL terra_schedule_dep_run(terra_schedule const * const base)
{
	terra_schedule *dep;

	if (SCHEDULE_INDEPENDENT(base))
		return TRUE;

	dep = schedule_dep(base);

	if (SCHEDULE_NOT_RUN(dep))
	{
		if (SCHEDULE_IS_CLOCK(dep))
		{
			terra_schedule_clock_run((terra_schedule_clock*) dep, dep);
		}
		else if (SCHEDULE_IS_HYGRO(dep))
		{
			terra_schedule_hygro_run((terra_schedule_hygro*) dep, dep);
		}
	}

	if (INVERS_DEP(base))
	{
		if (RUNTIME_SWITCH_OFF(dep->socket))
			return TRUE;
	}
	if (RUNTIME_SWITCH_ON(dep->socket))
		return TRUE;

	return FALSE;
}

BOOL terra_schedule_dep_init(terra_schedule const * const base)
{
	terra_schedule *dep;
	BOOL result;

	if (SCHEDULE_INDEPEND(base))
		return TRUE;

	dep = schedule_dep(base);

	if (SCHEDULE_IS_CLOCK(dep))
	{
		result = terra_schedule_clock_init((terra_schedule_clock*) dep);
	}
	else if (SCHEDULE_IS_HYGRO(dep))
	{
		result = terra_schedule_hygro_init((terra_schedule_hygro*) dep);
	}

	return INVERS_DEP(base) ? !result : result;
}

void terra_schedule_init()
{
	terra_schedule_clock *clock;
	terra_schedule_hygro *hygro;

	size_t i;

	terra_pin_set_out(CONF_LED.err_pin);
	terra_pin_set_out(CONF_LED.heart_pin);
	terra_pin_set_out(CONF_SWITCH.pin);

	terra_led_set(CONF_LED.heart_pin, FALSE);
	terra_led_set(CONF_LED.err_pin, FALSE);

	if (terra_mode_read(&RUNTIME_MODE))
	{
		terra_log_info("[terra_schedule_init] mode: %s\n", RUNTIME_MODE);
	}
	else
	{
		RUNTIME_DEFAULT_MODE();
	}

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		clock = SCHEDULE_GET_CLOCK(i);
		terra_schedule_clock_init(clock);
	}

	for (i = 0; i < CONF_GLOBAL.hygro_len; i++)
	{
		hygro = SCHEDULE_GET_HYGRO(i);
		terra_schedule_hygro_init(hygro);
	}

	runtime.switch_modes[0] = SWITCH_UNKNOWN;
	runtime.switch_modes[1] = SWITCH_UNKNOWN;
	runtime.switch_modes[2] = SWITCH_UNKNOWN;
}