#include "terra_hygro.h"

#include <time.h>

#define TIME struct tm
#define FORMAT_PATH "/var/opt/terra/hygro_%zu.%zu.%zu"

static terra_time _last_write = { 0, 0, 0 };

inline static TIME sys_time()
{
	time_t t = time(NULL);
	return *localtime(&t);
}

static BOOL terra_hygro_fwrite(float const h, float const f)
{
	static char buf[40];

	TIME syst;
	FILE *f;

	syst = sys_time();
	sprintf(buf, FORMAT_PATH, syst.tm_tmday, syst.tm_mon + 1, syst.tm_year);

	f = fopen(buf, "a+");
	if (!f)
	{
		terra_log_error("unable to open file %s\n");
		return FALSE;
	}

	fprintf(f, "%zu:%zu:%zu h: %f.2 t: %f.2\n", tm.tm_hour, tm.tm_min, tm.tm_sec, h, t);

	fclose(f);
	return TRUE;
}

BOOL terra_hygro_write(terra_conf const * const conf, terra_time const * const sys, float const h, float const f)
{
	size_t diff;

	diff = terra_time_diff(sys, &_last_write);

	if (diff < conf->hygro_write_secs)
		return TRUE;

	return terra_hygro_fwrite(h, f);
}