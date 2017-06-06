#ifndef __P_TERRA_H
#define __P_TERRA_H

#define BOOL int
#define TRUE 1
#define FALSE 0

#include <stdlib.h>

typedef struct
{
	char sock;
	BOOL set_on;
} terra_switch_req;

#endif