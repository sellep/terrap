#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#define BOOL int

typedef struct
{
	int pin;
	int repeats;
	int channel;
	int code_aon;
	int code_aoff;
	int code_bon;
	int code_boff;
	int code_con;
	int code_coff;
} terra_conf_switch;

typedef struct
{
	ssize_t delay;
	BOOL read_only;
	terra_conf_switch sw;
} terra_conf;

static inline void terra_conf_global_parse(terra_conf * const dest, config_t const * const src)
{
	config_lookup_bool(src, "read_only", &dest->read_only);
	config_lookup_int(src, "delay", &dest->delay);
}

static inline void terra_conf_switch_parse(terra_conf * const dest, config_t const * const src)
{ 
	config_lookup_int(src, "switch.pin", &(dest->sw.pin));
	config_lookup_int(src, "switch.repeats", &(dest->sw.repeats));
	config_lookup_int(src, "switch.channel", &(dest->sw.channel));
	config_lookup_int(src, "switch.code_aon", &(dest->sw.code_aon));
	config_lookup_int(src, "switch.code_aoff", &(dest->sw.code_aoff));
	config_lookup_int(src, "switch.code_bon", &(dest->sw.code_bon));
	config_lookup_int(src, "switch.code_boff", &(dest->sw.code_boff));
	config_lookup_int(src, "switch.code_con", &(dest->sw.code_con));
	config_lookup_int(src, "switch.code_coff", &(dest->sw.code_coff));
}

static inline void terra_conf_parse(terra_conf * const dest, config_t const * const src)
{
	terra_conf_global_parse(dest, src);
	terra_conf_switch_parse(dest, src);
}

static inline BOOL terra_conf_read(config_t * const conf)
{
	if (config_read_file(conf, "../res/terra.lib.conf"))
		return 1;

	fprintf(stderr, "%s:%d - %s\n", config_error_file(conf), config_error_line(conf), config_error_text(conf));
	config_destroy(conf);
	return 0;
}

static inline void terra_print_conf(terra_conf const * const c)
{
	printf("delay = %zu\n", c->delay);
	printf("read_only = %i\n", c->read_only);
	printf("###### switch ######\n");
	printf("pin = %zu\n", c->sw.pin);
	printf("repeats = %zu\n", c->sw.repeats);
	printf("channel = %zu\n", c->sw.channel);
	printf("code_aon = %zu\n", c->sw.code_aon);
	printf("code_aoff = %zu\n", c->sw.code_aoff);
	printf("code_bon = %zu\n", c->sw.code_bon);
	printf("code_boff = %zu\n", c->sw.code_boff);
	printf("code_con = %zu\n", c->sw.code_con);
	printf("code_coff = %zu\n", c->sw.code_coff);
}

int main()
{
	terra_conf dest;
	config_t conf;

	config_init(&conf);

	if (!terra_conf_read(&conf))
		exit(1);

	terra_conf_parse(&dest, &conf);
	terra_print_conf(&dest);

	config_destroy(&conf);
	return 0;
}