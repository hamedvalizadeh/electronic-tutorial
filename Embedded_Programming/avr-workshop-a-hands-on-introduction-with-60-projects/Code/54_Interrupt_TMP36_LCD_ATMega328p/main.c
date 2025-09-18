#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd_lq.h"
#include "adc.h"
#include <math.h>
#include <avr/interrupt.h>
#include "tmp36.h"
#include <stdio.h>

void display_temp(uint16_t adc_val)
{
    float temp = tmp36_convert(adc_val);

    char buffer[10];
    // Format with 2 decimal places (e.g., "26.17")
    dtostrf(temp, 6, 2, buffer);

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("TemT:");
    lcd_set_cursor(6, 0);
    lcd_print(buffer);

    _delay_ms(1000);
}

void set_configs()
{
    TMP36_CONFIG.vref_mv = 5000;
    TMP36_CONFIG.precision = TMP36_PRECISION_10BIT;
    TMP36_CONFIG.unit = TMP36_UNIT_FAHRENHEIT;

    LCD_CONFIG.port = &PORTD;
    LCD_CONFIG.ddr = &DDRD;
    LCD_CONFIG.rs = PORTD0;
    LCD_CONFIG.en = PORTD1;
    LCD_CONFIG.d4 = PORTD4;
    LCD_CONFIG.d5 = PORTD5;
    LCD_CONFIG.d6 = PORTD6;
    LCD_CONFIG.d7 = PORTD7;
    LCD_CONFIG.cols = 16;
    LCD_CONFIG.rows = 2;

    ADC_CONFIG.ref = ADC_REF_AVCC;
    ADC_CONFIG.prescaler = ADC_PRESCALER_8;
    ADC_CONFIG.adjust = ADC_LEFT_ADJUST;
    ADC_CONFIG.channel = 5;
    ADC_CONFIG.intrruptable = true;
    ADC_CONFIG.ten_bit = true;
}

int main(void)
{
    set_configs();

    lcd_init();

    adc_init();
    adc_set_callback(display_temp);

    sei();

    adc_start();

    while (1)
    {
    }

    return 0;
}