static int *file;

BOOL terra_switch_init(terra_switch_mode * * const modes, terra_conf_switch const * const conf)
{
	file = shm_open(SWITCH_FILE, O_CREAT | O_TRUNC | O_RDWR, S_IRWXU | S_IRWXG | S_IROTH);
	if (file < 0)
	{
		terra_log_error("[terra_switch_init] failed to create shared file (%s)\n", strerror(errno));
		file = NULL;
		return FALSE;
	}

	modes[0] = mmap(NULL, sizeof(terra_switch_modes) * 3, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
	if (modes[0] == MAP_FAILED)
	{
		terra_log_error("[terra_switch_init] failed to map shared file (%s)\n", strerror(errno));
		return FALSE;
	}

	return TRUE;
}