#ifndef __P_TERRA_SCHEDULE_H
#define __P_TERRA_SCHEDULE_H

#include "../terra_runtime.h"
#include "../utils/terra_daemon.h"

typedef terra_conf_schedule terra_schedule;
typedef terra_conf_schedule_clock terra_schedule_clock;
typedef terra_conf_schedule_hygro terra_schedule_hygro;
typedef terra_conf_schedule_period terra_schedule_period;

extern BOOL terra_schedule_arg(int const, char * * const);
extern terra_schedule_reset();

extern void terra_schedule_init();
extern BOOL terra_schedule_init_clock(terra_schedule_clock * const);
extern BOOL terra_schedule_init_hygro(terra_schedule_hygro * const);
extern BOOL terra_schedule_init_period(terra_schedule_period * const);

extern void terra_schedule_run();
void terra_schedule_run_clock(terra_schedule_clock * const, terra_schedule * const);
void terra_schedule_run_hygro(terra_schedule_hygro * const, terra_schedule * const);
void terra_schedule_run_period(terra_schedule_period * const, terra_schedule * const);

extern BOOL terra_schedule_dep_init(terra_schedule const * const);
extern BOOL terra_schedule_dep_run(terra_schedule const * const);

#define SCHEDULE_IS_INITIALIZED(s) (s)->run == TRUE
#define SCHEDULE_INITIALIZED(s) (s)->run = TRUE

#define SCHEDULE_IS_CLOCK(s) (s)->type == SCHEDULE_CLOCK
#define SCHEDULE_IS_HYGRO(s) (s)->type == SCHEDULE_HYGRO
#define SCHEDULE_IS_HYGRO(s) (s)->type == SCHEDULE_PERIOD

#define SCHEDULE_GET_CLOCK(i) (&CONF_GLOBAL.clocks[(i)])
#define SCHEDULE_GET_HYGRO(i) (&CONF_GLOBAL.hygros[(i)])
#define SCHEDULE_GET_PERIOD(i) (&CONF_GLOBAL.periods[(i)])

#define SCHEDULE(s) (&((s)->schedule))
#define SCHEDULE_ENABLED(s) (s)->enabled
#define SCHEDULE_DISABLED(s) !(SCHEDULE_ENABLED(s))
#define SCHEDULE_DISABLE(s) (s)->enabled = FALSE

#define SCHEDULE_NOT_RUN(s) (s)->run == FALSE
#define SCHEDULE_RUN_FINISHED(s) (s)->run = TRUE

#define SCHEDULE_SWITCH_SET_ON(s) terra_schedule_set_switch_on(s)
#define SCHEDULE_SWITCH_SET_OFF(s) terra_schedule_set_switch_off(s)

#define SCHEDULE_DEP_INIT(s) terra_schedule_dep_init(s)
#define SCHEDULE_DEPENDENT(s) (s)->dep != NULL
#define SCHEDULE_INDEPENDENT(s) (s)->dep == NULL
#define SCHEDULE_DEP_RUN(s) terra_schedule_dep_run(s)

static inline void terra_schedule_set_switch_on(terra_schedule const * const sched)
{
	terra_runtime_switch_set_on(sched->socket, sched->channel, sched->name);
}

static inline void terra_schedule_set_switch_off(terra_schedule const * const sched)
{
	terra_runtime_switch_set_off(sched->socket, sched->channel, sched->name);
}

#endif
