#ifndef PI_2_DHT_READ_H
#define PI_2_DHT_READ_H

#include "common_dht_read.h"

int dht_read(ssize_t const pin, float * const humidity, float * const temperature);

#endif