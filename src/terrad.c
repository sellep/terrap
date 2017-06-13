#include "terra.h"

extern BOOL terra_init();
extern BOOL terrad_run(terra_conf const * const);

int main(int argc, char **argv)
{
	terra_conf conf;
	BOOL run_result;

	terra_log_info("terra daemon started\n");

//initialization and setup
	if (!terra_init())
	{
		terra_log_error("failed to initialize terra\n");
		return 1;
	}

	if (argc != 2)
	{
		terra_log_error("insufficient argument count\n");
		terra_led_set(LED_PIN_ERR, TRUE);
		return 1;
	}

	terra_log_info("config file path: %s\n", argv[1]);

	if (!terra_conf_read(&conf, argv[1]))
	{
		terra_log_error("failed to read config file\n");
		terra_led_set(LED_PIN_ERR, TRUE);
		return 1;
	}

	terra_pin_set_output(conf.led_pin_heart);
	terra_pin_set_output(conf.switch_pin);

	terra_led_set(LED_PIN_ERR, FALSE);
	terra_led_set(conf.led_pin_heart, FALSE);

//run
	run_result = terrad_run(&conf);
	terra_log_info("terra daemon stopped\n");

	terra_led_set(LED_PIN_ERR, TRUE);

	if (!run_result)
		return 1;
	return 0;
}
