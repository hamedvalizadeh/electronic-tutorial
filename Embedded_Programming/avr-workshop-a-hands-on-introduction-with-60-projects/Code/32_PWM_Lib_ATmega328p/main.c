#include <avr/io.h>
#include <util/delay.h>
#include "avr8_timer.h"

int main(void)
{
    avr8_timer_0_config_t timer0_cfg = {
        .output = PWM0_OC0A,
        .mode = PWM0_FAST,
        .prescaler = 8,
        .duty_percent = 0,
        .inverting = true};

    avr8_timer_1_config_t timer1_cfg = {
        .output = PWM1_OC1A,
        .mode = PWM1_FAST_8BIT,
        .prescaler = 8,
        .duty_percent = 0,
        .inverting = false};

    avr8_timer_2_config_t timer2_cfg = {
        .output = PWM2_OC2A,
        .mode = PWM2_FAST,
        .prescaler = 1,
        .duty_percent = 0,
        .inverting = false};

    avr8_timer_0_init(&timer0_cfg);
    avr8_timer_1_init(&timer1_cfg);
    avr8_timer_2_init(&timer2_cfg);

    avr8_timer_0_set_duty(&timer0_cfg, 0);
    avr8_timer_1_set_duty(&timer1_cfg, 0);
    avr8_timer_2_set_duty(&timer2_cfg, 0);

    // HINT: try with value
    uint16_t top0 = avr8_timer_0_get_top();
    uint16_t top1 = avr8_timer_1_get_top();
    uint16_t top2 = avr8_timer_2_get_top();
    uint16_t pulse_ticks;

    // HINT: try with percent
    uint8_t duty_percent;
    while (1)
    {
        for (duty_percent = 0; duty_percent <= 100; duty_percent++)
        {
            avr8_timer_0_set_duty(&timer0_cfg, duty_percent);
            avr8_timer_1_set_duty(&timer1_cfg, duty_percent);
            avr8_timer_2_set_duty(&timer2_cfg, duty_percent);
            _delay_ms(8);
        }

        for (duty_percent = 100; duty_percent > 0; duty_percent--)
        {
            avr8_timer_0_set_duty(&timer0_cfg, duty_percent);
            avr8_timer_1_set_duty(&timer1_cfg, duty_percent);
            avr8_timer_2_set_duty(&timer2_cfg, duty_percent);
            _delay_ms(8);
        }

        avr8_timer_0_set_duty(&timer0_cfg, 0);
        avr8_timer_1_set_duty(&timer1_cfg, 0);
        avr8_timer_2_set_duty(&timer2_cfg, 0);

        _delay_ms(5000);
        for (pulse_ticks = 0; pulse_ticks <= top0; pulse_ticks++)
        {
            avr8_timer_0_set_pulse_ticks(&timer0_cfg, pulse_ticks);
            _delay_ms(8);
        }
        for (pulse_ticks = 0; pulse_ticks <= top1; pulse_ticks++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, pulse_ticks);
            _delay_ms(8);
        }
        for (pulse_ticks = 0; pulse_ticks <= top2; pulse_ticks++)
        {
            avr8_timer_2_set_pulse_ticks(&timer2_cfg, pulse_ticks);
            _delay_ms(8);
        }

        for (pulse_ticks = top0; pulse_ticks > 0; pulse_ticks--)
        {
            avr8_timer_0_set_pulse_ticks(&timer0_cfg, pulse_ticks);
            _delay_ms(8);
        }
        for (pulse_ticks = top1; pulse_ticks > 0; pulse_ticks--)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, pulse_ticks);
            _delay_ms(8);
        }
        for (pulse_ticks = top2; pulse_ticks > 0; pulse_ticks--)
        {
            avr8_timer_2_set_pulse_ticks(&timer2_cfg, pulse_ticks);
            _delay_ms(8);
        }
    }

    return 0;
}