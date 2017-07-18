#ifndef __P_TERRA_SCHEDULE_H
#define __P_TERRA_SCHEDULE_H

#include "../terra_runtime.h"

extern void terra_schedule_run();

extern void terra_schedule_init_temp(terra_schedule_temp * const);

extern void terra_schedule_run_clock(terra_schedule_clock * const);
extern void terra_schedule_run_period(terra_schedule_period * const);
extern void terra_schedule_run_temp(terra_schedule_temp * const);
extern BOOL terra_schedule_depcheck(terra_schedule const * const);

#define SCHEDULE_IS_CLOCK(s) (s)->type == SCHEDULE_CLOCK
#define SCHEDULE_IS_PERIOD(s) (s)->type == SCHEDULE_PERIOD
#define SCHEDULE_IS_TEMP(s) (s)->type == SCHEDULE_TEMP

#define SCHEDULE_GET_CLOCK(i) (&CONF_GLOBAL.clocks[(i)])
#define SCHEDULE_GET_PERIOD(i) (&CONF_GLOBAL.periods[(i)])
#define SCHEDULE_GET_TEMP(i) (&CONF_GLOBAL.temps[(i)])

#define SCHEDULE(s) ((terra_schedule*)(s))
#define SCHEDULE_DISABLED(s) !((s)->enabled)

#define SCHEDULE_NO_DEP(s) (s)->dep == NULL
#define SCHEDULE_INVERS_DEP(s) (s)->name[0] == "!"

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
}

#endif
