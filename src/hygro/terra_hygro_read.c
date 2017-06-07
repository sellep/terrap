#include "terra_hygro.h"

// Number of bit pulses to expect from the DHT.  Note that this is 41
// because the first pulse is a constant 50 microsecond pulse, with 40
// pulses to represent the data afterwards.
#define HYGRO_DHT_PULSES 41

// 
#defin HYGRO_TIMEOUT_THRESHOLD 32000

//maximum amount of time to spin in a loop before bailing out and considering the read a timeout
#define BUSY_WAIT_COUNT 50

static int _pulse_count[HYGRO_DHT_PULSES * 2];


BOOL terra_hygro_read(terra_hygro_res * const res, terra_conf const * const conf)
{
	ssize_t i;
	//Set pin to output.
	//pi_2_mmio_set_output(conf->hygro_pin);

	//Bump up process priority and change scheduler to try to make process more 'real time'.
	//set_max_priority();

	// Set pin high for ~500 milliseconds.
	//pi_2_mmio_set_high(pin);
	//sleep_milliseconds(500);

	//Set pin low for ~20 milliseconds.
	//pi_2_mmio_set_low(pin);
	//busy_wait_milliseconds(20);

	//Set pin to input.
	//pi_2_mmio_set_input(conf->hygro_pin);

	//Need a very short delay before reading pins or else value is sometimes still low.
	for (volatile int j = 0; j < BUSY_WAIT_COUNT; j++);

	//wait for DHT to pull pin low
	//i = 0;
	//while (pi_2_mmio_input(conf->pin))
	//{
		//if (++count == HYGRO_TIMEOUT_THRESHOLD)
		//{
		//	set_default_priority();
		//	terra_log_error("hygro read timed out\n");
		//	return FALSE;
		//}
	//}

	return TRUE;
}