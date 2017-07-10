#include "terra_runtime.h"

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define LOCK_FILE "terra_lock"

static int _sm_mutex;
static volatile pthread_mutex_t *_mutex;
static pthread_mutexattr_t _mutex_attr;

extern BOOL terra_lock_init();
extern BOOL map_mutex_to_address_space();


BOOL terra_runtime_init(char const * const conf_path)
{
	if (!terra_lock_init())
	{
		terra_log_error("[terra_runtime_init] failed to initialize lock\n");
		return FALSE;
	}

	if (!terra_conf_read(&conf, conf_path))
	{
		terra_log_error("[terra_runtime_init] failed to read config file\n");
		return FALSE;
	}

	runtime.hygro_err = 0;

	terra_pin_out(conf.switch_pin);
	terra_pin_out(conf.led_pin_heart);
	terra_pin_out(conf.led_pin_alert);

	runtime.tick = 0;
	terra_time_now(&runtime.now);

	return TRUE;
}


BOOL terra_lock_init()
{
	_sm_mutex = shm_open(LOCK_FILE, O_CREAT | O_EXCL | O_TRUNC | O_RDWR, S_IRWXU | S_IRWXG);
	if (_sm_mutex < 0)
	{
		if (errno == EEXIST)
		{
			_sm_mutex = shm_open(LOCK_FILE, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);

			if (_sm_mutex < 0)
			{
				terra_log_error("failed to open existing shared memory object (%s)\n", strerror(errno));
				return FALSE;
			}

			return map_mutex_to_address_space();
		}

		terra_log_error("failed to create shared memory object (%s)\n", strerror(errno));
		return 0;
	}


	if (ftruncate(_sm_mutex, sizeof(pthread_mutex_t)) == -1)
	{
		terra_log_error("failed to truncate shared memory object (%s)\n", strerror(errno));
		return 0;
	}

	if (!map_mutex_to_address_space())
		return FALSE;

	//init mutex
	pthread_mutexattr_init(&_mutex_attr);
	pthread_mutexattr_setpshared(&_mutex_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(_mutex, &_mutex_attr);
	return TRUE;
}

BOOL map_mutex_to_address_space()
{
	_mutex = (pthread_mutex_t*) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, _sm_mutex, 0);
	if (_mutex == MAP_FAILED)
	{
		terra_log_error("failed to map memory\n");
		return FALSE;
	}

	return TRUE;
}
