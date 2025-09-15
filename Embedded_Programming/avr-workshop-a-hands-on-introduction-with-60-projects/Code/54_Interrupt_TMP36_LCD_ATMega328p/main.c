#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd_lq.h"
#include "adc.h"
#include <math.h>
#include <avr/interrupt.h>

uint8_t adc_to_temp_celcius(uint16_t adc_val)
{
    float volt = (adc_val * 5);
    volt = volt / 1024;
    float temp = ((volt - 0.5) * 100);
    return ((uint8_t)round(temp));
}

float adc_to_temp_celcius_float(uint16_t adc_val)
{
    float volt = (adc_val * 5);
    volt = volt / 1024;
    float temp = ((volt - 0.5) * 100);
    return temp;
}

void adc__interrupt_handler(uint16_t adc_val)
{
    float temp = adc_to_temp_celcius_float(adc_val);
    char formated[10];

    dtostrf(temp, 9, 3, formated);

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Temp_1:");
    lcd_set_cursor(7, 0);
    lcd_print(formated);

    _delay_ms(1000);
}

int main(void)
{
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
    lcd_init();

    adc_init(ADC_REF_AVCC, ADC_PRESCALER_8);
    adc_select_channel(5);
    adc_enable_interrupt(true);
    adc_set_callback(adc__interrupt_handler);

    sei(); // enable global interrupts
    adc_start(); // start first conversion

    while (1)
    {
    }

    return 0;
}