#ifndef MAX7219_H
#define MAX7219_H

#include <avr/io.h>

void max7219_write(uint8_t address, uint8_t data);
void max7219_setup();
void max7219_blank();
void max7219_display_number(uint8_t digit, uint8_t number, uint8_t dp);
void max7219_send_number(uint32_t value);

#endif
