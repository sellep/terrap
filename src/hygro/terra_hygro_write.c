#include "terra_hygro.h"

#include <time.h>

#define DATE struct tm
#define FORMAT_PATH "/var/opt/terra/hygro_%zu.%zu.%zu"

static terra_time _last_write = { 0, 0, 0 };

inline static DATE sys_date()
{
	time_t t = time(NULL);
	return *localtime(&t);
}

static BOOL terra_hygro_fwrite(float const h, float const t)
{
	static char buf[40];

	DATE d;
	FILE *f;

	d = sys_date();
	sprintf(buf, FORMAT_PATH, d.tm_mday, d.tm_mon + 1, d.tm_year);

	f = fopen(buf, "a+");
	if (!f)
	{
		terra_log_error("unable to open file %s\n");
		return FALSE;
	}

	fprintf(f, "%zu:%zu:%zu h: %f.2 t: %f.2\n", d.tm_hour, d.tm_min, d.tm_sec, h, t);

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