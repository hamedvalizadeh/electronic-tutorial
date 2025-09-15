#include "adc.h"
#include <util/delay.h>

void adc_init(adc_ref_t ref, adc_prescaler_t prescaler)
{
    // Clear reference selection bits
    ADMUX &= ~((1 << REFS1) | (1 << REFS0));
    ADMUX |= (ref << REFS0); // Set reference

    // Enable ADC
    ADCSRA |= (1 << ADEN);

    // Set prescaler (division factor for ADC clock)
    ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
    ADCSRA |= prescaler;
}

void adc_select_channel(uint8_t channel)
{
    if (channel > 7)
        return; // Only ADC0–ADC7 on ATmega328P

    // Clear previous channel selection
    ADMUX &= 0xF0; // Keep reference bits, clear MUX[3:0]
    ADMUX |= channel;
}

uint16_t adc_read(void)
{
    // Start conversion
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to finish
    while (ADCSRA & (1 << ADSC))
        ;
    _delay_ms(5);
    // Return ADC value (10-bit)
    return ADC;
}
