#ifndef __P_TERRA_SCHEDULE_H
#define __P_TERRA_SCHEDULE_H

#include "../terra_runtime.h"

extern void terra_schedule_arg(int const, char const * const * const);
extern void terra_schedule_run();

extern void terra_schedule_init_clock(terra_schedule_clock * const);
extern void terra_schedule_init_temp(terra_schedule_temp * const);
extern void terra_schedule_init_period(terra_schedule_period * const);

extern void terra_schedule_run_clock(terra_schedule_clock * const);
extern void terra_schedule_run_temp(terra_schedule_temp * const);
extern void terra_schedule_run_period(terra_schedule_period * const);

extern BOOL terra_schedule_dep_enabled(terra_schedule const * const);
extern BOOL terra_schedule_dep_check(terra_schedule const * const);

#define SCHEDULE_IS_CLOCK(s) (s)->type == SCHEDULE_CLOCK
#define SCHEDULE_IS_TEMP(s) (s)->type == SCHEDULE_TEMP
#define SCHEDULE_IS_PERIOD(s) (s)->type == SCHEDULE_PERIOD

#define SCHEDULE_GET_CLOCK(i) (&CONF_GLOBAL.clocks[(i)])
#define SCHEDULE_GET_TEMP(i) (&CONF_GLOBAL.temps[(i)])
#define SCHEDULE_GET_PERIOD(i) (&CONF_GLOBAL.periods[(i)])

#define SCHEDULE(s) (&((s)->schedule))
#define SCHEDULE_ENABLED(s) (s)->enabled
#define SCHEDULE_DISABLED(s) !(SCHEDULE_ENABLED(s))

#define SCHEDULE_NO_DEP(s) (s)->dep == NULL
#define SCHEDULE_INVERS_DEP(s) (s)->dep[0] == '!'

#define SCHEDULE_NOT_RUN(s) (s)->run == FALSE

static inline void schedule_reset()
{
	ssize_t i;

	for (i = 0; i < CONF_GLOBAL.clock_len; i++)
	{
		SCHEDULE(SCHEDULE_GET_CLOCK(i))->run = FALSE;
	}

	for (i = 0; i < CONF_GLOBAL.temp_len; i++)
	{
		SCHEDULE(SCHEDULE_GET_TEMP(i))->run = FALSE;
	}

	for (i = 0; i < CONF_GLOBAL.period_len; i++)
	{
		SCHEDULE(SCHEDULE_GET_PERIOD(i))->run = FALSE;
	}
}

static inline void terra_exit(int const code)
{
	//TODO: set led alert on
	exit(code);
}

#endif
