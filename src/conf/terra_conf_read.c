#include "terra_conf.h"

extern BOOL terra_conf_read_global(terra_conf * const conf, FILE * const f);
extern BOOL terra_conf_read_sched_clocks(terra_conf * const, FILE * const);

BOOL terra_conf_read(terra_conf * const conf, char const * const path)
{
	FILE *f;

	f = fopen(path, "r");
	if (!f)
	{
		terra_log_error("unable to read config file\n");
		return FALSE;
	}

	if (!terra_conf_read_global(conf, f))
	{
		fclose(f);
		return FALSE;
	}

	printf("terra_conf_read_global\n");

	rewind(f);

	if (!terra_conf_read_sched_clocks(conf, f))
	{
		fclose(f);
		return FALSE;
	}

	printf("terra_conf_read_sched_clocks\n");

	fclose(f);
	return TRUE;
}
