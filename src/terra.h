#ifndef __P_TERRA_H
#define __P_TERRA_H

#define BOOL int
#define TRUE 1
#define FALSE 0

typedef struct
{
	char sock;
	BOOL set_on;
} terra_switch_req;

BOOL terra_setup(terra_conf const * const);

#endif