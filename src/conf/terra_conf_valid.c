#include "terra_conf.h"

static inline BOOL terra_conf_valid_clock(terra_sched_clock * const clock)
{
	ssize_t i;

	if (strlen(clock->sched.name) == 0)
	{
		terra_log_error("[terra_conf_valid_clock] clock->sched.name cannot be empty\n");
		return FALSE;
	}

	if (!clock->sched.enabled)
		return TRUE;

	if (clock->times_len == 5)
	{
		clock->sched.enabled = FALSE;
		terra_log_info("[terra_conf_valid_clock] disabled clock schedule %s\n", clock->sched.name);
		return TRUE;
	}

	for (i = 0; i < clock->times_len; i++)
	{
		if (terra_time_cmp(&(clock->times[i].stop), &(clock->times[i].start)) != TIME_ABOVE)
		{
			terra_log_error("[terra_conf_valid_clock] clock->times[%zu].start cannot be after stop\n", i);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL terra_conf_valid(terra_conf * const conf)
{
	ssize_t i;

	if (conf->tick == 0)
	{
		terra_log_error("[terra_conf_valid] tick cannot be zero\n");
		return FALSE;
	}

	if (conf->heart_tick == 0)
	{
		terra_log_error("[terra_conf_valid] heart_tick cannot be zero\n");
		return FALSE;
	}

	if (conf->heart_dur == 0)
	{
		terra_log_error("[terra_conf_valid] heart_dur cannot be zero\n");
		return FALSE;
	}

	if (conf->switch_pin == conf->led_pin_heart)
	{
		terra_log_error("[terra_conf_valid] switch_pin not available\n");
		return FALSE;
	}

	if (conf->switch_rep == 0)
	{
		terra_log_error("[terra_conf_valid] switch_rep cannot be zero\n");
		return FALSE;
	}

	if (conf->switch_chan == 0)
	{
		terra_log_error("[terra_conf_valid] switch_chan cannot be zero\n");
		return FALSE;
	}

	if (conf->switch_code_aon == 0)
	{
		terra_log_error("[terra_conf_valid] switch_code_aon cannot be zero\n");
		return FALSE;
	}

	if (conf->switch_code_aoff == 0)
	{
		terra_log_error("[terra_conf_valid] switch_code_aoff cannot be zero\n");
		return FALSE;
	}

	if (conf->switch_code_bon == 0)
	{
		terra_log_error("[terra_conf_valid] switch_code_bon cannot be zero\n");
		return FALSE;
	}

	if (conf->switch_code_boff == 0)
	{
		terra_log_error("[terra_conf_valid] switch_code_boff cannot be zero\n");
		return FALSE;
	}

	if (conf->switch_code_con == 0)
	{
		terra_log_error("[terra_conf_valid] switch_code_con cannot be zero\n");
		return FALSE;
	}

	if (conf->switch_code_coff == 0)
	{
		terra_log_error("[terra_conf_valid] switch_code_coff cannot be zero\n");
		return FALSE;
	}

	if (conf->hygro_enabled)
	{
		if (conf->hygro_pin_io == conf->led_pin_heart)
		{
			terra_log_error("[terra_conf_valid] hygro_pin_io not available\n");
			return FALSE;
		}

		if (conf->hygro_pin_io == conf->switch_pin)
		{
			terra_log_error("[terra_conf_valid] hygro_pin_io not available\n");
			return FALSE;
		}

		if (conf->hygro_tick == 0)
		{
			terra_log_error("[terra_conf_valid] hygro_tick cannot be zero\n");
			return FALSE;
		}
	}

	for (i = 0; i < conf->sched_clocks_len; i++)
	{
		if (!terra_conf_valid_clock(&(conf->sched_clocks[i])))
			return FALSE;
	}

	return TRUE;
}
