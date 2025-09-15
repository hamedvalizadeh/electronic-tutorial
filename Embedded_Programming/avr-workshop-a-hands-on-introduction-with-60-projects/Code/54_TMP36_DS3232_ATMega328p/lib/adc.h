#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// ADC reference voltage options
typedef enum
{
    ADC_REF_AREF = 0,
    ADC_REF_AVCC = 1,
    ADC_REF_INTERNAL = 3 // 1.1V internal reference
} adc_ref_t;

// ADC prescaler options
typedef enum
{
    ADC_PRESCALER_2 = 1,
    ADC_PRESCALER_4 = 2,
    ADC_PRESCALER_8 = 3,
    ADC_PRESCALER_16 = 4,
    ADC_PRESCALER_32 = 5,
    ADC_PRESCALER_64 = 6,
    ADC_PRESCALER_128 = 7
} adc_prescaler_t;

// Initialize ADC with reference and prescaler
void adc_init(adc_ref_t ref, adc_prescaler_t prescaler);

// Select ADC channel (0–7)
void adc_select_channel(uint8_t channel);

// Read ADC value (blocking)
uint16_t adc_read(void);

#endif // ADC_H
