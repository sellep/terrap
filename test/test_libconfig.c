#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#define BOOL int

typedef struct
{
	ssize_t delay;
	BOOL read_only;
} terra_global_conf;

static inline BOOL terra_conf_read(config_t * const conf)
{
	if (config_read_file(conf, "../res/terra.lib.conf"))
		return 1;

	fprintf(stderr, "%s:%d - %s\n", config_error_file(conf), config_error_line(conf), config_error_text(conf));
	config_destroy(conf);
	return 0;
}

static inline void terra_print_conf(terra_global_conf const * const c)
{
	printf("[delay] %zu\n", c->delay);
	printf("[read_only] %i\n", c->read_only);
}

int main()
{
	terra_global_conf global;
	config_t conf;

	config_init(&conf);

	if (!terra_conf_read(&conf))
		exit(1);

	config_lookup_bool(&conf, "read_only", &global.read_only);
	config_lookup_int(&conf, "delay", &global.delay);

	config_destroy(&conf);
	terra_print_conf(&global);
	return 0;
}