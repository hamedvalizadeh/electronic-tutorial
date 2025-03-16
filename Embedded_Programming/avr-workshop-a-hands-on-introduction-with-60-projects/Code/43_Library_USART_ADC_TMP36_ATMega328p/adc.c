#include <avr/io.h>
#include <util/delay.h>

void setup_port_adc()
{
    DDRC &= ~(1 << PORTC5);
}

void setup_adc5_avcc()
{
    // select AVCC (pin 20 of the ATMega328p) as the voltage source for ADC circuit.
    ADMUX |= (1 << REFS0);

    // select ADC5 (pin 28 of the ATMega328p)
    ADMUX |= (1 << MUX2) | (1 << MUX0);

    // set ADC pre-scalar as 8
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0);

    // enable ADC
    ADCSRA |= (1 << ADEN);
}

uint16_t read_value_adc5()
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & ((1 << ADSC)))
        ;
    _delay_ms(10);
    return ADC;
}