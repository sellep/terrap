#include "terra_switch.h"

static int file;

static inline BOOL switch_open_file()
{
	BOOL do_truncate = TRUE;

	file = shm_open(SWITCH_FILE, O_EXCL | O_CREAT | O_TRUNC | O_RDWR, S_IRWXU | S_IRWXG | S_IROTH);

	if (errno == EEXIST)
	{
		do_truncate = FALSE;

		file = shm_open(SWITCH_FILE, O_CREAT | O_TRUNC | O_RDWR, S_IRWXU | S_IRWXG | S_IROTH);
	}

	if (file < 0)
	{
		terra_log_error("[terra_switch_init] failed to create shared file (%s)\n", strerror(errno));
		return FALSE;
	}

	if (do_truncate)
	{
		printf("do truncate\n");

		if (ftruncate(file, sizeof(terra_switch_mode) * 3) == -1)
		{
			terra_log_error("[terra_switch_init] failed to truncate shared file (%s)\n", strerror(errno));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL terra_switch_init(terra_switch_mode * * const modes, terra_conf_switch const * const conf)
{
	if (!switch_open_file())
	{
		terra_log_error("[terra_switch_init] failed to create shared file (%s)\n", strerror(errno));
		return FALSE;
	}

	printf("initialized\n");

	if ((modes[0] = mmap(NULL, sizeof(terra_switch_mode) * 3, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0)) == MAP_FAILED)
	{
		terra_log_error("[terra_switch_init] failed to map shared file (%s)\n", strerror(errno));
		return FALSE;
	}

	printf("mapped\n");

	return TRUE;
}