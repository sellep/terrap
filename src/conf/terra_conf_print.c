#include "terra_conf.h"

void terra_conf_print(terra_conf const * const conf)
{
	size_t i, j;

	terra_log_info("terra_conf:\n");
	terra_log_info("[global] tick: %u\n", conf->tick);
	terra_log_info("[global] led_pin_warn: %u\n", conf->led_pin_warn);
	terra_log_info("[global] switch_pin: %u\n", conf->switch_pin);
	terra_log_info("[global] switch_rep: %u\n", conf->switch_rep);
	terra_log_info("[global] switch_chan: %u\n", conf->switch_chan);
	terra_log_info("[global] switch_code_aon: %u\n", conf->switch_code_aon);
	terra_log_info("[global] switch_code_aoff: %u\n", conf->switch_code_aoff);
	terra_log_info("[global] switch_code_bon: %u\n", conf->switch_code_bon);
	terra_log_info("[global] switch_code_boff: %u\n", conf->switch_code_boff);
	terra_log_info("[global] switch_code_con: %u\n", conf->switch_code_con);
	terra_log_info("[global] switch_code_coff: %u\n", conf->switch_code_coff);
	terra_log_info("[global] hygro_enabled: %u\n", conf->hygro_enabled);
	terra_log_info("[global] hygro_pin_io: %u\n", conf->hygro_pin_io);
	terra_log_info("[global] hygro_tick: %u\n", conf->hygro_tick);
	terra_log_info("[global] hygro_sensor: %s\n", conf->hygro_sensor);

	for (i = 0; i < conf->sched_clocks_len; i++)
	{
		terra_log_info("[sched_clock %u] name: %s\n", i, conf->sched_clocks[i].sched.name);
		terra_log_info("[sched_clock %u] trigger: %s\n", i,
			conf->sched_clocks[i].sched.trig == TRIGGER_CLOCK ? "clock"
			: conf->sched_clocks[i].sched.trig == TRIGGER_TEMP ? "temp"
			: "mois");
		terra_log_info("[sched_clock %u] sock: %c\n", i, conf->sched_clocks[i].sched.sock);
		terra_log_info("[sched_clock %u] enabled: %u\n", i, conf->sched_clocks[i].sched.enabled);

		terra_log_info("[sched_clock %u] times: %u\n", i, &conf->sched_clocks[i].times_len);

		for (j = 0; j < &conf->sched_clocks[i].times_len; j++)
		{
			terra_log_info("[sched_clock %u] start: ", i);
			terra_time_print(&(conf->sched_clocks[i].times[j].start));
			terra_log_info("[sched_clock %u] stop: ", i);
			terra_time_print(&(conf->sched_clocks[i].times[j].stop));
		}
	}
}
