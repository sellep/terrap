#include "terra_conf.h"

extern BOOL terra_conf_read_global(terra_conf * const, FILE * const);
extern BOOL terra_conf_read_sched_clocks(terra_conf * const, FILE * const);
extern BOOL terra_conf_read_sched_periods(terra_conf * const, FILE * const);
extern BOOL terra_conf_read_sched_temps(terra_conf * const, FILE * const);

BOOL terra_conf_read(terra_conf * const conf, char const * const path)
{
	FILE *f;

	f = fopen(path, "r");
	if (!f)
	{
		terra_log_error("[terra_conf_read] unable to read config file %s\n", path);
		return FALSE;
	}

	if (!terra_conf_read_global(conf, f))
	{
		fclose(f);
		return FALSE;
	}

	rewind(f);

	if (!terra_conf_read_sched_periods(conf, f))
	{
		fclose(f);
		return FALSE;
	}

	rewind(f);

	if (!terra_conf_read_sched_temps(conf, f))
	{
		fclose(f);
		return FALSE;
	}

	rewind(f);

	if (!terra_conf_read_sched_clocks(conf, f))
	{
		fclose(f);
		return FALSE;
	}

	fclose(f);
	return TRUE;
}
