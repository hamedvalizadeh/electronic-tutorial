// lcd_lq.h
#ifndef LCDLQ_H
#define LCDLQ_H

#include <avr/io.h>

void lcd_latche_data();
void lcd_write(uint8_t data);
void lcd_cmd(uint8_t cmd);
void lcd_init();
void lcd_print(char *data);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_clear();

typedef struct
{
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    uint8_t rs, en, d4, d5, d6, d7;
    uint8_t cols;
    uint8_t rows;
} lcd_config_t;

extern lcd_config_t LCD_CONFIG;

#endif