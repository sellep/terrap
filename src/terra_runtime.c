#include "terra_runtime.h"

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define LOCK_FILE "terra_lock"

static int _sm_mutex;
static pthread_mutexattr_t _mutex_attr;

inline BOOL map_mutex_to_address_space()
{
	runtime.mutex = (pthread_mutex_t*) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, _sm_mutex, 0);
	if (runtime.mutex == MAP_FAILED)
	{
		terra_log_error("[map_mutex_to_address_space] failed to map memory\n");
		return FALSE;
	}

	return TRUE;
}

static BOOL terra_lock_init()
{
	_sm_mutex = shm_open(LOCK_FILE, O_CREAT | O_EXCL | O_TRUNC | O_RDWR, S_IRWXU | S_IRWXG);
	if (_sm_mutex < 0)
	{
		if (errno == EEXIST)
		{
			_sm_mutex = shm_open(LOCK_FILE, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);

			if (_sm_mutex < 0)
			{
				terra_log_error("[terra_lock_init] failed to open existing shared memory object (%s)\n", strerror(errno));
				return FALSE;
			}

			return map_mutex_to_address_space();
		}

		terra_log_error("[terra_lock_init] failed to create shared memory object (%s)\n", strerror(errno));
		return 0;
	}


	if (ftruncate(_sm_mutex, sizeof(pthread_mutex_t)) == -1)
	{
		terra_log_error("[terra_lock_init] failed to truncate shared memory object (%s)\n", strerror(errno));
		return 0;
	}

	if (!map_mutex_to_address_space())
		return FALSE;

	//init mutex
	pthread_mutexattr_init(&_mutex_attr);
	pthread_mutexattr_setpshared(&_mutex_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(runtime.mutex, &_mutex_attr);
	return TRUE;
}

BOOL terra_runtime_init(char const * const conf_path)
{
#ifndef DEBUG
	if (pi_2_mmio_init() != MMIO_SUCCESS)
	{
		terra_log_error("[terra_runtime_init] failed to initialize mmio\n");
		return FALSE;
	}
#endif

	if (!terra_lock_init())
	{
		terra_log_error("[terra_runtime_init] failed to initialize lock\n");
		return FALSE;
	}

	if (!terra_conf_read(&(CONF_GLOBAL), conf_path))
	{
		terra_log_error("[terra_runtime_init] failed to read config file\n");
		return FALSE;
	}
	
	printf("CONFIG GLOBAL delay %i\n", CONF_GLOBAL.delay);

	runtime.hygro_err = 0;

	terra_pin_set_out(CONF_SWITCH.pin);
	terra_pin_set_out(CONF_HEART.pin);
	terra_pin_set_out(CONF_GLOBAL.pin_alert);

	runtime.tick = 0;
	terra_time_now(&runtime.now);

	return TRUE;
}
