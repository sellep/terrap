#include "terra.h"

extern BOOL terrad_run();

int main(int argc, char **argv)
{
	BOOL run_result;

	terra_log_info("terra daemon started\n");

//initialization and setup
	if (!terra_runtime_init(TERRA_CONF_PATH))
	{
		terra_log_error("[terrad] failed to initialize runtime\n");
		exit(1);
	}

	terra_pin_set_output(conf.led_pin_alert);
	terra_pin_set_output(conf.led_pin_heart);
	terra_pin_set_output(conf.switch_pin);

	terra_led_set(conf.led_pin_alert, FALSE);
	terra_led_set(conf.led_pin_heart, FALSE);

//run
	run_result = terrad_run();

	terra_log_info("terra daemon stopped\n");
	terra_led_set(conf.led_pin_alert, TRUE);

	if (!run_result)
		return 1;
	return 0;
}
