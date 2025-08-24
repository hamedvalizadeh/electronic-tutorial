// spi.h
#ifndef CONVERSION_H
#define CONVERSION_H

#include <avr/io.h>

uint8_t dec_to_bcd(uint8_t val);
uint8_t bcd_to_dec(uint8_t val);

#endif
