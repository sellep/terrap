#ifndef COMMON_DHT_READ_H
#define COMMON_DHT_READ_H

#define DHT_ERROR_TIMEOUT -1
#define DHT_ERROR_CHECKSUM -2
#define DHT_ERROR_ARGUMENT -3
#define DHT_ERROR_GPIO -4
#define DHT_SUCCESS 0

void busy_wait_milliseconds(size_t const);
void sleep_milliseconds(size_t const);
void set_max_priority();
void set_default_priority();

#endif