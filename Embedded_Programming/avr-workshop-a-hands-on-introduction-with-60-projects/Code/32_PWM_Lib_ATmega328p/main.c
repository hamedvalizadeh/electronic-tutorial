#include <avr/io.h>
#include <util/delay.h>
#include "avr8_timer.h"

int main(void)
{
    avr8_timer_1_config_t timer1_cfg = {
        .output = PWM1_OC1A,
        .mode = PWM1_FAST_8BIT,
        .prescaler = 8,
        .duty = 0,
        .inverting = false};

    avr8_timer_1_init(&timer1_cfg);

    //HINT: try with value
    uint16_t top = avr8_timer_1_get_top();
    uint16_t duty;
    while (1)
    {
        for (duty = 0; duty <= top; duty++)
        {
            avr8_timer_1_set_duty(&timer1_cfg, duty);
            _delay_ms(8);
        }

        for (duty = top; duty > 0; duty--)
        {
            avr8_timer_1_set_duty(&timer1_cfg, duty);
            _delay_ms(8);
        }
    }

    //HINT: try with percent
    uint8_t duty_percent;
    while (1)
    {
        for (duty_percent = 0; duty_percent <= 100; duty_percent++)
        {
            avr8_timer_1_set_duty_percent(&timer1_cfg, duty_percent);
            _delay_ms(8);
        }

        for (duty_percent = 100; duty_percent > 0; duty_percent--)
        {
            avr8_timer_1_set_duty_percent(&timer1_cfg, duty_percent);
            _delay_ms(8);
        }
    }

    return 0;
}