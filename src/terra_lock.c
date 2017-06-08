#include "terra.h"

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

#define LOCK_FILE "terra_lock"

static int _sm_mutex;
static pthread_mutex_t *_mutex;
static pthread_mutexattr_t _mutex_attr;

BOOL terra_lock_init()
{
	_sm_mutex = shm_open(LOCK_FILE, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG);
	if (_sm_mutex < 0)
	{
		terra_log_error("failed to create shared memory object (%s)\n", strerror(errno));
		return FALSE;
	}

	if (ftruncate(_sm_mutex, sizeof(pthread_mutex_t)) == -1)
	{
		terra_log_error("failed to truncate shared memory object\n");
		return FALSE;
	}

	_mutex = (pthread_mutex_t*) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, _sm_mutex, 0);
	if (_mutex == MAP_FAILED)
	{
		terra_log_error("failed to map memory\n");
		return FALSE;
	}

	pthread_mutexattr_init(&_mutex_attr);
	pthread_mutexattr_setpshared(&_mutex_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(_mutex, &_mutex_attr);

	return TRUE;
}

void terra_lock()
{
	pthread_mutex_lock(_mutex)
}

void terra_unlock()
{
	pthread_mutex_unlock(_mutex)
}