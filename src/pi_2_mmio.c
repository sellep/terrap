#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "pi_2_mmio.h"

#define GPIO_BASE_OFFSET 0x200000
#define GPIO_LENGTH 4096

volatile size_t *pi_2_mmio_gpio = NULL;

mmio_status pi_2_mmio_init()
{
	unsigned char buf[4];
	size_t peri_base;
	size_t gpio_base;

	FILE *fp = fopen("/proc/device-tree/soc/ranges", "rb");
	if (fp == NULL)
		return MMIO_ERROR_OFFSET;

	fseek(fp, 4, SEEK_SET);

	if (fread(buf, 1, sizeof(buf), fp) != sizeof(buf))
		return MMIO_ERROR_OFFSET;

	peri_base = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0;
	gpio_base = peri_base + GPIO_BASE_OFFSET;

	fclose(fp);

	int fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
	if (fd == -1)
		return MMIO_ERROR_DEVMEM;

	pi_2_mmio_gpio = (size_t*)mmap(NULL, GPIO_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, gpio_base);

	close(fd);

	if (pi_2_mmio_gpio == MAP_FAILED)
		return MMIO_ERROR_MMAP;

	return MMIO_SUCCESS;
}