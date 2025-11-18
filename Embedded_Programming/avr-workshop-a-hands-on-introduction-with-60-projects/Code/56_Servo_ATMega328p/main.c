#include <avr/io.h>
#include <util/delay.h>
#include "avr8_timer.h"

int main(void)
{
    avr8_timer_1_config_t timer1_cfg = {
        .output = PWM1_OC1A,
        .mode = PWM1_FAST_8BIT,
        .prescaler = 8,
        .duty_percent = 0,
        .inverting = false};

    avr8_timer_1_init(&timer1_cfg);
    
    uint8_t duty;

    while (1)
    {
        for (duty = 0; duty <= 100; duty++)
        {
            avr8_timer_1_set_duty_percent(&timer1_cfg, duty);
            _delay_ms(8);
        }

        for (duty = 100; duty > 0; duty--)
        {
            avr8_timer_1_set_duty_percent(&timer1_cfg, duty);
            _delay_ms(8);
        }
    }

    return 0;
}