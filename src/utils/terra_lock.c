#include "terra_lock.h"

#define LOCK_FILE "terra_lock"

static int sm_mutex;
static pthread_mutexattr_t mutex_attr;

static inline BOOL map_mutex_to_address_space()
{
	mutex = (pthread_mutex_t*) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, sm_mutex, 0);
	if (mutex == MAP_FAILED)
	{
		terra_log_error("[map_mutex_to_address_space] failed to map memory\n");
		return FALSE;
	}

	return TRUE;
}

BOOL terra_lock_init()
{
	sm_mutex = shm_open(LOCK_FILE, O_CREAT | O_EXCL | O_TRUNC | O_RDWR, S_IRWXU | S_IRWXG);
	if (sm_mutex < 0)
	{
		if (errno == EEXIST)
		{
			sm_mutex = shm_open(LOCK_FILE, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);

			if (sm_mutex < 0)
			{
				terra_log_error("[terra_lock_init] failed to open existing shared memory object (%s)\n", strerror(errno));
				return FALSE;
			}

			return map_mutex_to_address_space();
		}

		terra_log_error("[terra_lock_init] failed to create shared memory object (%s)\n", strerror(errno));
		return 0;
	}

	if (ftruncate(sm_mutex, sizeof(pthread_mutex_t)) == -1)
	{
		terra_log_error("[terra_lock_init] failed to truncate shared memory object (%s)\n", strerror(errno));
		return 0;
	}

	if (!map_mutex_to_address_space())
		return FALSE;

	pthread_mutexattr_init(&mutex_attr);
	pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(mutex, &mutex_attr);
	return TRUE;
}