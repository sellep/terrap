#include "terra_mode.h"

#include "terra_lock.h"

#define ARG_MODE_WRITE "write"
#define MODE_FILE "/var/lib/terra/mode"

extern BOOL terra_mode_write(terra_mode);

BOOL terra_mode_run(int const argc, char * * const argv)
{
	terra_mode mode;

	if (argc == 2)
	{
		if (!terra_mode_read(&mode))
		{
			terra_log_error("[terra_mode_run] failed to read group\n");
			return FALSE;
		}

		terra_log_info("[terra_mode_run] %s\n", mode);
		free(mode);
	}
	else if (argc == 4)
	{
		if (strcmp(argv[2], ARG_MODE_WRITE) != 0)
		{
			terra_log_error("[terra_mode_run] invalid mode %s\n", argv[2]);
			return FALSE;
		}

		return terra_mode_write(argv[3]);
	}
	else
	{
		terra_log_error("[terra_mode_run] invalid mode args\n");
		return FALSE;
	}

	return TRUE;
}

BOOL terra_mode_write(terra_mode mode)
{
	char buf[MODE_NAME_MAX + 1];
	int mode_h;

	sprintf(buf, "%s\n", mode);

	LOCK();

	mode_h = open(MODE_FILE, O_WRONLY | O_CREAT | O_TRUNC, USER_WRITE);

	if (mode_h == -1)
	{
		terra_log_error("[terra_mode_write] failed to create/open mode file\n");
		UNLOCK();
		return FALSE;
	}

	write(mode_h, buf, strlen(buf));
	close(mode_h);

	UNLOCK();

	return TRUE;
}

BOOL terra_mode_read(terra_mode * const mode)
{
	char buf[MODE_NAME_MAX + 1];
	BOOL status;
	int mode_h;

	status = TRUE;

	LOCK();

	mode_h = open(MODE_FILE, O_RDONLY);
	if (mode_h == -1)
	{
		terra_log_error("[terra_mode_read] failed to open mode file\n");
		UNLOCK();
		return FALSE;
	}

	if (read(mode_h, buf, sizeof buf) == -1)
	{
		terra_log_error("[terra_mode_read] failed to read mode file\n");
		status = FALSE;
	}

	close(mode_h);

	UNLOCK();

	mode[0] = (char*) malloc(sizeof(char) * MODE_NAME_MAX);
	strcpy(mode[0], buf);
	mode[0][strlen(buf) - 1] = '\0';

	return status;
}