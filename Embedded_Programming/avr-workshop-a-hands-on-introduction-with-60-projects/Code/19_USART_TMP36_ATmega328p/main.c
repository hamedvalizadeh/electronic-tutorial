// Project 19 – Digital thermometer with terminal output

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <util/delay.h>

#define USART_BAUDRATE 4800
#define UBRR_VALUE 12

void setup_adc()
{
    // Set External Voltage Source
    ADMUX |= (1 << REFS0);
    // Select ADC5 chanell (PC5 - pin number 28)
    ADMUX |= (1 << MUX2) | (1 << MUX0);

    // enable ADC
    ADCSRA |= (1 << ADEN);
    // Set pre-scalar to 8 (we use internal 1 MHZ crystal, so the speed of 125 KHZ for ADC would be suitable)
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
}

uint16_t read_adc()
{
    // start ADC
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {
    }
    _delay_ms(5);
    return ADC;
}

uint8_t convert_adc_to_temprature_celcius(uint16_t adc_value)
{
    float voltage = (adc_value * 5);
    voltage = voltage / 1024;
    voltage = voltage - 0.5;
    voltage = voltage * 100;
    return (uint8_t)round(voltage);
}

int main(void)
{
    uint16_t adc_value;
    uint8_t temp_value;
    DDRC = 0b00000000;

    setup_adc();

    while (1)
    {
        adc_value = read_adc();
        temp_value = convert_adc_to_temprature_celcius(adc_value);
        _delay_ms(1000);
    }

    return 0;
}
