#include "terra_group.h"

#include "terra_lock.h"

#define ARG_MODE_WRITE "write"
#define GROUP_FILE "/var/lib/terra/group"

extern BOOL terra_group_write(char const * const);

BOOL terra_group_run(int const argc, char * * const argv)
{
	terra_group group;

	if (argc == 2)
	{
		if (!terra_group_read(&group))
		{
			terra_log_error("[terra_group_run] failed to read group\n");
			return FALSE;
		}

		terra_log_info("[terra_group_run] %s\n", group.name);
	}
	else if (argc == 4)
	{
		if (strcmp(argv[2], ARG_MODE_WRITE) != 0)
		{
			terra_log_error("[terra_group_run] invalid group mode %s\n", argv[2]);
			return FALSE;
		}

		return terra_group_write(argv[3]);
	}
	else
	{
		terra_log_error("[terra_group_run] invalid group args\n");
		return FALSE;
	}

	return TRUE;
}

BOOL terra_group_write(char const * const name)
{
	char buf[GROUP_NAME_MAX + 1];
	int group_h;

	sprintf(buf, "%s\n", name);

	LOCK();

	group_h = open(GROUP_FILE, O_WRONLY | O_CREAT | O_TRUNC , OWNER_READ_WRITE);

	if (group_h == -1)
	{
		terra_log_error("[terra_group_write] failed to create/open group file\n");
		UNLOCK();
		return FALSE;
	}

	write(group_h, buf, strlen(buf));
	close(group_h);

	UNLOCK();

	return TRUE;
}

BOOL terra_group_read(terra_group * const group)
{
	char buf[GROUP_NAME_MAX + 1];
	BOOL status;
	int group_h;
	int n;

	status = TRUE;

	LOCK();

	group_h = open(GROUP_FILE, O_RDONLY);
	if (group_h == -1)
	{
		terra_log_error("[terra_group_read] failed to open group file\n");
		UNLOCK();
		return FALSE;
	}

	if (read(group_h, buf, sizeof buf) == -1)
	{
		terra_log_error("[terra_group_read] failed to read group file\n");
		status = FALSE;
	}

	close(group_h);

	UNLOCK();

	n = strcpn(buf, "\n");
	strncpy(group->name, buf, n);

	return status;
}
