#include "terra.h"

#include "conf/terra_conf.h"

extern BOOL terrad_run(terra_conf const * const);
extern void terrad_led_set(ssize_t const, BOOL const);

int main(int argc, char **argv)
{
	terra_conf conf;
	BOOL run_result;

	terra_log_info("terra daemon started\n");

	terra_pin_set_output(LED_PIN_ERR);

	if (argc != 2)
	{
		terra_log_error("insufficient argument count\n");
		terrad_led_set(LED_PIN_ERR, TRUE);
		return 1;
	}

	terra_log_info("config file path: %s\n", argv[1]);

//initialization and setup
	if (!terra_lock_init())
	{
		terra_log_error("failed to initialize terra lock\n");
		terrad_led_set(LED_PIN_ERR, TRUE);
		return 1;
	}

	if (!terra_conf_read(&conf, argv[1]))
	{
		terra_log_error("failed to read config file\n");
		terrad_led_set(LED_PIN_ERR, TRUE);
		return 1;
	}

	terra_pin_set_output(conf.led_pin_warn);
	terra_pin_set_output(conf.switch_pin);

	terrad_led_set(LED_PIN_ERR, FALSE);
	terrad_led_set(conf.led_pin_warn, FALSE);

//run
	run_result = terrad_run(&conf);
	terra_log_info("terra daemon stopped\n");

	terrad_led_set(LED_PIN_ERR, TRUE);

	if (!run_result)
		return 1;
	return 0;
}
