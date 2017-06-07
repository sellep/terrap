#include "terra_conf.h"

void terra_conf_print(terra_conf const * const conf)
{
	size_t i;

	terra_log_info("terra_conf:\n");
	terra_log_info("[global] tick: %u\n", conf->tick);
	terra_log_info("[global] tran_pin: %u\n", conf->tran_pin);
	terra_log_info("[global] tran_rep: %u\n", conf->tran_rep);
	terra_log_info("[global] sock_chan: %u\n", conf->sock_chan);
	terra_log_info("[global] sock_code_a_on: %u\n", conf->sock_code_a_on);
	terra_log_info("[global] sock_code_a_off: %u\n", conf->sock_code_a_off);
	terra_log_info("[global] sock_code_b_on: %u\n", conf->sock_code_b_on);
	terra_log_info("[global] sock_code_b_off: %u\n", conf->sock_code_b_off);
	terra_log_info("[global] sock_code_c_on: %u\n", conf->sock_code_c_on);
	terra_log_info("[global] sock_code_c_off: %u\n", conf->sock_code_c_off);
	terra_log_info("[global] hygro_enabled: %u\n", conf->hygro_enabled);
	terra_log_info("[global] hygro_pin: %u\n", conf->hygro_pin);
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
		terra_log_info("[sched_clock %u] start: ", i);
		terra_time_print(&conf->sched_clocks[i].start);
		terra_log_info("[sched_clock %u] end: ", i);
		terra_time_print(&conf->sched_clocks[i].end);
	}
}
