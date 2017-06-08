#include "terra.h"

#define TERRA_LOCK_FILE_HYGRO "/var/run/dht.lock"
#define TERRA_LOCK_FILE_SWITCH "/var/run/433tran.lock"

void terra_lock(terra_lock_mode const mode)
{
	if (mode & TERRA_SWITCH)
	{
		
	}
}