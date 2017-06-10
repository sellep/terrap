#ifndef PI_2_MMIO_H
#define PI_2_MMIO_H

#include <stdint.h>

#define MMIO_SUCCESS 0
#define MMIO_ERROR_DEVMEM -1
#define MMIO_ERROR_MMAP -2
#define MMIO_ERROR_OFFSET -3

typedef int mmio_status;

extern volatile size_t *pi_2_mmio_gpio;

mmio_status pi_2_mmio_init();

// Set GPIO register to 000 for specified GPIO number.
static inline void pi_2_mmio_set_input(const int gpio_number)
{
	*(pi_2_mmio_gpio + ((gpio_number) / 10)) &= ~(7 << (((gpio_number) % 10) * 3));
}

static inline void pi_2_mmio_set_output(const int gpio_number)
{
	// First set to 000 using input function.
	pi_2_mmio_set_input(gpio_number);
	// Next set bit 0 to 1 to set output.
	*(pi_2_mmio_gpio + ((gpio_number)/10)) |=  (1 << (((gpio_number) % 10) * 3));
}

static inline void pi_2_mmio_set_high(int const gpio_number)
{
	*(pi_2_mmio_gpio + 7) = 1 << gpio_number;
}

static inline void pi_2_mmio_set_low(int const gpio_number)
{
	*(pi_2_mmio_gpio + 10) = 1 << gpio_number;
}

static inline size_t pi_2_mmio_input(int const gpio_number)
{
	return *(pi_2_mmio_gpio + 13) & (1 << gpio_number);
}

#endif