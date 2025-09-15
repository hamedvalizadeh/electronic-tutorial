#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

// ADC callback type
typedef void (*adc_callback_t)(uint16_t result);

// ADC reference voltage options
typedef enum
{
#if defined(REFS1) && defined(REFS0)
    ADC_REF_AREF = 0,
    ADC_REF_AVCC = 1,
    ADC_REF_INTERNAL = 3
#else
    ADC_REF_DEFAULT = 0
#endif
} adc_ref_t;

// ADC prescaler options
typedef enum
{
#if defined(ADPS2)
    ADC_PRESCALER_2 = 1,
    ADC_PRESCALER_4 = 2,
    ADC_PRESCALER_8 = 3,
    ADC_PRESCALER_16 = 4,
    ADC_PRESCALER_32 = 5,
    ADC_PRESCALER_64 = 6,
    ADC_PRESCALER_128 = 7
#else
    ADC_PRESCALER_DEFAULT = 0
#endif
} adc_prescaler_t;

// Initialize ADC
void adc_init(adc_ref_t ref, adc_prescaler_t prescaler);

// Select ADC channel (0..MAX)
void adc_select_channel(uint8_t channel);

// Read ADC value (blocking)
uint16_t adc_read(void);

// Register a user callback for ADC interrupt
void adc_set_callback(adc_callback_t cb);

// Start ADC conversion (non-blocking)
void adc_start(void);

// Enable/disable ADC interrupt
void adc_enable_interrupt(bool enable);

// Check if conversion finished (polling)
bool adc_conversion_done(void);

#endif // ADC_H