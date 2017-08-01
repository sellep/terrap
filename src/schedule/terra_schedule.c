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
			terra_schedule_run_clock((terra_schedule_clock*) dep, dep);
		}
		else if (SCHEDULE_IS_HYGRO(dep))
		{
			terra_schedule_run_hygro((terra_schedule_hygro*) dep, dep);
		}
		else if (SCHEDULE_IS_PERIOD(dep))
		{
			terra_schedule_run_period((terra_schedule_period*) dep, dep);
		}
		else
		{
			return FALSE;
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

	if (SCHEDULE_INDEPENDENT(base))
		return TRUE;

	dep = schedule_dep(base);

	if (SCHEDULE_IS_CLOCK(dep))
		return terra_schedule_init_clock((terra_schedule_clock*) dep);

	if (SCHEDULE_IS_HYGRO(dep))
		return terra_schedule_init_hygro((terra_schedule_hygro*) dep);

	if (SCHEDULE_IS_PERIOD(dep))
		return terra_schedule_init_period((terra_schedule_period*) dep);

	return FALSE;
}

void terra_schedule_reset()
{
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		SCHEDULE(SCHEDULE_GET_CLOCK(i))->run = FALSE;
	}

	for (i = 0; i < CONF_GLOBAL.hygro_len; i++)
	{
		SCHEDULE(SCHEDULE_GET_HYGRO(i))->run = FALSE;
	}

	for (i = 0; i < CONF_GLOBAL.period_len; i++)
	{
		SCHEDULE(SCHEDULE_GET_PERIOD(i))->run = FALSE;
	}
}

void terra_schedule_init()
{
	size_t i;
	printf("init\n");

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

	terra_schedule_reset();

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		terra_schedule_init_clock(SCHEDULE_GET_CLOCK(i));
	}

	for (i = 0; i < CONF_GLOBAL.hygro_len; i++)
	{
		terra_schedule_init_hygro(SCHEDULE_GET_HYGRO(i));
	}

	for (i = 0; i < CONF_GLOBAL.period_len; i++)
	{
		terra_schedule_init_period(SCHEDULE_GET_PERIOD(i));
	}

	runtime.switch_modes[0] = SWITCH_UNKNOWN;
	runtime.switch_modes[1] = SWITCH_UNKNOWN;
	runtime.switch_modes[2] = SWITCH_UNKNOWN;
}