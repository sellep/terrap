#include "terra_conf.h"

#include <stdio.h>

extern BOOL terra_conf_read_glob_impl(terra_conf * const, FILE * const);

BOOL terra_conf_read_glob(terra_conf * const conf, char const * const path)
{
	FILE *f;
	BOOL impl;

	f = fopen(path, "r");
	if (!f)
	{
		fprintf(stderr, "unable to read config file\n");
		return FALSE;
	}

	impl = terra_conf_read_glob_impl(conf, f);

	fclose(f);

	return impl;
}
