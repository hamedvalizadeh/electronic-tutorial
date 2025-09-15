#include "adc.h"
#include <util/delay.h>

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
    // Clear reference selection bits if present
#if defined(REFS1) && defined(REFS0)
    ADMUX &= ~((1 << REFS1) | (1 << REFS0));
    ADMUX |= (ref << REFS0);
#endif
#endif

#if defined(ADCSRA)
    // Enable ADC
    ADCSRA |= (1 << ADEN);

    // Set prescaler if available
#if defined(ADPS2)
    ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
    ADCSRA |= prescaler;
#endif
#endif
}

void adc_select_channel(uint8_t channel)
{
    if (channel >= ADC_MAX_CHANNELS)
        return; // prevent out-of-bounds

#if defined(ADMUX)
#if defined(MUX0)
    // Clear previous channel bits
    ADMUX &= 0xF0;
    ADMUX |= channel & 0x0F;
#endif
#endif
}

uint16_t adc_read(void)
{
#if defined(ADCSRA) && defined(ADSC)
    // Start conversion
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to finish
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
