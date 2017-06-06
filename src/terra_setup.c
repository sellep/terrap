#include "terra.h"

#include "switch/terra_switch.h"

void terra_setup(terra_conf const * const cfg)
{
	terra_switch_set_pin(cfg->tran_pin)
}