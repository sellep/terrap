#ifndef __P_TERRA_UTILS_GROUP_H
#define __P_TERRA_UTILS_GROUP_H

#include "../terra_defs.h"

#define GROUP_NAME_MAX 20

typedef struct
{
	char name[GROUP_NAME_MAX];
} terra_group;

extern BOOL terra_group_run(int const, char * * const);
extern BOOL terra_group_read(terra_group * const);

#endif
