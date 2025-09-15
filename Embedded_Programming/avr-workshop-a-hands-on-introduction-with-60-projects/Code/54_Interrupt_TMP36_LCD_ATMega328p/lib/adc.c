#include "adc.h"
#include <util/delay.h>
#include <avr/interrupt.h>

// Detect number of ADC channels based on device
#if defined(ADC)
#define ADC_MAX_CHANNELS 16
#elif defined(ADC0)
#define ADC_MAX_CHANNELS 8
#else
#define ADC_MAX_CHANNELS 4
#endif

void adc_init(adc_ref_t ref, adc_prescaler_t prescaler)
{
#if defined(ADMUX)
#if defined(REFS1) && defined(REFS0)
    ADMUX &= ~((1 << REFS1) | (1 << REFS0));
    ADMUX |= (ref << REFS0);
#endif
#endif

#if defined(ADCSRA)
    ADCSRA |= (1 << ADEN); // Enable ADC
#if defined(ADPS2)
    ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
    ADCSRA |= prescaler;
#endif
#endif
}

void adc_select_channel(uint8_t channel)
{
    if (channel >= ADC_MAX_CHANNELS)
        return;

#if defined(ADMUX)
#if defined(MUX0)
    ADMUX &= 0xF0;
    ADMUX |= channel & 0x0F;
#endif
#endif
}

uint16_t adc_read(void)
{
#if defined(ADCSRA) && defined(ADSC)
    ADCSRA |= (1 << ADSC); // Start conversion
    while (ADCSRA & (1 << ADSC))
        ;
#if defined(ADC)
    return ADC;
#else
    return 0;
#endif
#else
    return 0;
#endif
}

void adc_start(void)
{
#if defined(ADCSRA) && defined(ADSC)
    ADCSRA |= (1 << ADSC);
#endif
}

void adc_enable_interrupt(bool enable)
{
#if defined(ADCSRA) && defined(ADIE)
    if (enable)
    {
        ADCSRA |= (1 << ADIE); // Enable ADC interrupt
    }
    else
    {
        ADCSRA &= ~(1 << ADIE); // Disable ADC interrupt
    }
#endif
}

bool adc_conversion_done(void)
{
#if defined(ADCSRA) && defined(ADIF)
    return (ADCSRA & (1 << ADIF));
#else
    return false;
#endif
}

static adc_callback_t adc_user_callback = 0; // null by default

void adc_set_callback(adc_callback_t cb)
{
    adc_user_callback = cb;
}

ISR(ADC_vect)
{
    uint16_t result = ADC; // read ADC result directly

    if (adc_user_callback)
    {
        adc_user_callback(result); // call user handler
    }

    ADCSRA |= (1 << ADSC); // start next conversion
}
