#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define LOCK_FILE "terra_lock"

static int _sm_mutex;
static pthread_mutex_t *_mutex;
static pthread_mutexattr_t _mutex_attr;

#define LOCK()(pthread_mutex_lock(_mutex))
#define UNLOCK()(pthread_mutex_unlock(_mutex))

int init_lock()
{
	_sm_mutex = shm_open(LOCK_FILE, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG);
	if (_sm_mutex < 0)
	{
		fprintf(stderr, "failed to create shared memory object (%s)\n", strerror(errno));
		return 0;
	}

	if (ftruncate(_sm_mutex, sizeof(pthread_mutex_t)) == -1)
	{
		fprintf(stderr, "failed to truncate shared memory object\n");
		return 0;
	}

	_mutex = (pthread_mutex_t*) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, _sm_mutex, 0);
	if (_mutex == MAP_FAILED)
	{
		fprintf(stderr, "failed to map memory\n");
		return 0;
	}

	pthread_mutexattr_init(&_mutex_attr);
	pthread_mutexattr_setpshared(&_mutex_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(_mutex, &_mutex_attr);

	return 1;
}

void cleanup_lock()
{
	pthread_mutexattr_destroy(&_mutex_attr);
	pthread_mutex_destroy(_mutex);
	shm_unlink(LOCK_FILE);
}

int main()
{
	if (!init_lock())
	{
		exit(1);
	}
	
	if (fork() > 0)
	{
		//i'm the parent process, so let me sleep!
		sleep(10);

		LOCK();
		printf("hello from parent!\n");
		UNLOCK();

		cleanup_lock();
	}
	else
	{
		LOCK();
		printf("hello from child!\n");
		sleep(5000);
		UNLOCK();
	}

	return 0;
}
