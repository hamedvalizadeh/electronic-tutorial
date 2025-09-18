#include <avr/io.h>
#include <util/delay.h>
#include "avr8_timer.h"

int main(void)
{
    // Configure PWM for Timer1, OC1A (PB1), 8-bit Fast PWM, prescaler 8
    avr8_timer_1_config_t timer1_cfg = {
        .output = PWM1_OC1A,
        .mode = PWM1_FAST_8BIT, // Timer1 8-bit Fast PWM
        .prescaler = 8,
        .duty = 0,          // Initial duty
        .top = 0xFF,        // 8-bit Fast PWM top
        .inverting = false, // Non-inverting output
        .use_ICR_as_TOP = 0 // OCR1A used as TOP for this mode
    };

    // Initialize Timer1 PWM
    avr8_timer_1_init(&timer1_cfg);

    uint16_t duty;

    while (1)
    {
        // Ramp duty from 0 → top
        for (duty = 0; duty <= timer1_cfg.top; duty++)
        {
            avr8_timer_1_set_duty(&timer1_cfg, duty);
            _delay_ms(8);
        }

        // Ramp duty from top → 0
        for (duty = timer1_cfg.top; duty > 0; duty--)
        {
            avr8_timer_1_set_duty(&timer1_cfg, duty);
            _delay_ms(8);
        }
    }

    return 0;
}
