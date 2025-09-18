#ifndef TMP36_H
#define TMP36_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    TMP36_UNIT_CELSIUS,
    TMP36_UNIT_FAHRENHEIT
} tmp36_unit_t;

typedef enum
{
    TMP36_PRECISION_8BIT,   // 0..255 (left-adjusted or truncated)
    TMP36_PRECISION_10BIT   // 0..1023 (full ADC)
} tmp36_precision_t;

typedef struct
{
    uint16_t vref_mv;           // Reference voltage in millivolts (e.g., 5000 for 5.0V)
    tmp36_precision_t precision; // ADC precision: 8-bit or 10-bit
    tmp36_unit_t unit;           // Output in Celsius or Fahrenheit
} tmp36_config_t;

extern tmp36_config_t TMP36_CONFIG;

float tmp36_convert(uint16_t adc_val);

#endif // TMP36_H
