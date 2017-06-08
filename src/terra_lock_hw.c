#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

#define LOCK_FILE "/terra/lock"

static int sm_mutex;
static pthread_mutex_t *mutex;

int init_sm()
{
	sm_mutex = shm_open(LOCK_FILE, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG);
	if (sm_mutex < 0)
	{
		fprintf(stderr, "failed to create shared memory object (%u)\n", sm_mutex);
		return 0;
	}

	if (ftruncate(sm_mutex, sizeof(pthread_mutex_t)) == -1)
	{
		fprintf(stderr, "failed to truncate shared memory object\n");
		return 0;
	}

	mutex = (pthread_mutex_t*) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, sm_mutex, 0);
	if (mutex == MAP_FAILED)
	{
		fprintf(stderr, "failed to map memory\n");
		return 0;
	}
	return 1;
}

void cleanup_sm()
{
	pthread_mutex_destroy(mutex);
	shm_unlink(LOCK_FILE);
}

int main()
{
	if (!init_sm())
	{
		exit(1);
	}

	cleanup_sm();
	return 0;
}