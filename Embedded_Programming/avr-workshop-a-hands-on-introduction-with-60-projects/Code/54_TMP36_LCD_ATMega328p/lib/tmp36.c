#include "tmp36.h"
#include <math.h>

tmp36_config_t TMP36_CONFIG = {
    .vref_mv = 3300,
    .precision = TMP36_PRECISION_8BIT,
    .unit = TMP36_UNIT_FAHRENHEIT};

uint16_t get_resolution(void)
{
    switch (TMP36_CONFIG.precision)
    {
    case TMP36_PRECISION_8BIT:
        return 256;
    case TMP36_PRECISION_10BIT:
        return 1024;
    default:
        return 1024; // fallback
    }
}

float tmp36_convert(uint16_t adc_val)
{
    // Convert ADC count to millivolts (float to preserve fractions)
    float mv = ((float)adc_val * TMP36_CONFIG.vref_mv) / get_resolution();

    // Celsius = (mv - 500) / 10
    float temp_c = (mv - 500.0f) / 10.0f;

    if (TMP36_CONFIG.unit == TMP36_UNIT_FAHRENHEIT)
    {
        // F = C * 9/5 + 32
        temp_c = (temp_c * 9.0f / 5.0f) + 32.0f;
    }

    if (TMP36_CONFIG.precision == TMP36_PRECISION_8BIT)
    {
        // Round to nearest integer, drop decimals
        temp_c = roundf(temp_c);
    }

    return temp_c;
}