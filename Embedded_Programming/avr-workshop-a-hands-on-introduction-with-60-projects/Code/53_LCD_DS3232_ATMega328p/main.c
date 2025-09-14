#include <avr/io.h>
#include <util/delay.h>
#include "conversion.h"
#include "i2c.h"
#include "ds3232.h"
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
    LCD_CONFIG.rows = 2;
    lcd_init();

    I2C_CONFIG.scl_freq_hz = 100000;
    I2C_CONFIG.prescaler = 1;
    I2C_CONFIG.wait_ms = 15;
    I2C_CONFIG.retry_count = 5;
    i2c_init();

    // // HINT: uncomment following code to set time to ((2025-07-15 Tuesday, 14:36:45))
    // ds3232_set_time(45, 36, 14, 2, 15, 7, 25);
    // _delay_ms(1000);
    rtc_time_t current;

    char formated[9];

    while (1)
    {
        current = ds3232_read_date_and_time();

        get_formated_date(current, formated, sizeof(formated));
        lcd_set_cursor(0, 0);
        lcd_print("Date: ");
        lcd_set_cursor(6, 0);
        lcd_print(formated);

        get_formated_time(current, formated, sizeof(formated));
        lcd_set_cursor(0, 1);
        lcd_print("Time: ");
        lcd_set_cursor(6, 1);
        lcd_print(formated);

        _delay_ms(1000);
    }

    return 0;
}