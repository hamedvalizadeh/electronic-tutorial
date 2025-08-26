#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd_lq.h"

int main(void)
{
    LCD_CONFIG.port = &PORTD;
    LCD_CONFIG.ddr = &DDRD;
    LCD_CONFIG.rs = PORTD0;
    LCD_CONFIG.en = PORTB1;
    LCD_CONFIG.d4 = PORTD4;
    LCD_CONFIG.d5 = PORTD5;
    LCD_CONFIG.d6 = PORTD6;
    LCD_CONFIG.d7 = PORTD7;
    LCD_CONFIG.cols = 16;
    LCD_CONFIG.rows = 1;

    lcd_init();

    char numbers[9];
    uint8_t i;
    while (1)
    {
        lcd_set_cursor(1, 0);
        lcd_print("AVR Workshop");
        _delay_ms(1000);
        lcd_clear();

        lcd_set_cursor(0, 0);
        lcd_print("Learning LCD Use");
        _delay_ms(1000);
        lcd_clear();

        lcd_set_cursor(1, 0);
        lcd_print("Counting Up");
        _delay_ms(1000);
        lcd_clear();
        for (i = 0; i < 10; i++)
        {
            itoa(i, numbers, 10);
            lcd_set_cursor(i, 0);
            lcd_print(numbers);
            _delay_ms(1000);
        }
        lcd_clear();
    }

    return 0;
}