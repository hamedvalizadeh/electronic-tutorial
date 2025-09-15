#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

typedef void (*adc_callback_t)(uint16_t result);

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

typedef enum
{
    ADC_RIGHT_ADJUST = 0,
    ADC_LEFT_ADJUST = 1
} adc_adjust_t;

typedef struct
{
    adc_ref_t ref;
    adc_prescaler_t prescaler;
    adc_adjust_t adjust;
    uint8_t channel;
    bool intrruptable;
    bool ten_bit;
} adc_config_t;

void adc_init(void);
uint16_t adc_read_blocking(void);
uint16_t adc_read(void);
void adc_start(void);
void adc_enable_interrupt(bool enable);
bool adc_conversion_done(void);
void adc_set_callback(adc_callback_t cb);

extern adc_config_t ADC_CONFIG;

#endif // ADC_H