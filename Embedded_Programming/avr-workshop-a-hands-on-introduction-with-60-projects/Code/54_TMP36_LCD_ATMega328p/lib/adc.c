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

adc_config_t ADC_CONFIG = {
    .ref = ADC_REF_AVCC,
    .prescaler = ADC_PRESCALER_64,
    .adjust = ADC_RIGHT_ADJUST,
    .channel = 0,
    .intrruptable = false,
    .ten_bit = true};

void adc_init(void)
{
#if defined(ADMUX)
    // Set reference voltage
    ADMUX &= ~((1 << REFS1) | (1 << REFS0));
    ADMUX |= (ADC_CONFIG.ref << REFS0);

    // Set alignment
    if (ADC_CONFIG.adjust == ADC_LEFT_ADJUST)
        ADMUX |= (1 << ADLAR);
    else
        ADMUX &= ~(1 << ADLAR);

    // Select default channel
    ADMUX = (ADMUX & 0xF0) | (ADC_CONFIG.channel & 0x0F);
#endif

#if defined(ADCSRA)
    ADCSRA |= (1 << ADEN); // Enable ADC

#if defined(ADPS2)
    ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
    ADCSRA |= ADC_CONFIG.prescaler;
#endif
#endif

    if (ADC_CONFIG.intrruptable)
    {
        adc_enable_interrupt(true);
    }
}

uint16_t adc_read_blocking(void)
{
#if defined(ADCSRA) && defined(ADSC)
    adc_start();
    while (ADCSRA & (1 << ADSC))
        ;
    return adc_read();
#else
    return 0;
#endif
}

uint16_t adc_read(void)
{
    uint16_t result = 0;
    if (ADC_CONFIG.adjust == ADC_LEFT_ADJUST)
    {
#if defined(ADCH)
        uint8_t low = ADCL;
        uint8_t high = ADCH;
        if (ADC_CONFIG.ten_bit)
        {
            result = ((uint16_t)high << 2) | (low >> 6);
        }
        else
        {
            result = high;
        }
#endif
    }
    else
    {
#if defined(ADC)
        result = ADC;
#endif
    }

    return result;
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
    uint16_t result = adc_read();

    if (adc_user_callback)
    {
        adc_user_callback(result); // call user handler
    }

    ADCSRA |= (1 << ADSC); // start next conversion
}