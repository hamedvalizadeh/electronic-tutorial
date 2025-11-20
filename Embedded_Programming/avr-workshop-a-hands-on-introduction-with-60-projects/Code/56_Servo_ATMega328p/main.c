#include <avr/io.h>
#include <util/delay.h>
#include "avr8_timer.h"

int main(void)
{
    // HINT: we are simulating SG90 servo.
    //       this servo works with 50 Hz frequency.
    //       in SG90 servo, 0 degree is 1 ms and 180 degrees is 2 ms based on the datasheet.
    //       so to set the duty to rotate to desired angle, fist we should calculate each tick duration.
    //       tick = prescalar / f_cpu
    //       0 degree = 1 ms or 1000 us = 1000 us / tick
    //       180 degree = 2 ms or 2000 us = 2000 us / tick
    //       any angle = ((angle / 180) * 2000 us) / tick

    // tick = 127 us
    // 0 degree = 1000 us / 127 us = 7.874
    // 180 degree = 2000 us / 127 us = 15.74
    // each degree = (15 - 7) / 180 = 0.0437 (as we have 8 us interval and we can increment 1 us, so our precison is 23 degrees)
    avr8_timer_0_config_t timer0_cfg = {
        .output = PWM0_OC0B,
        .mode = PWM0_PHASE_CORRECT_OCR,
        .prescaler = 1024,
        .duty_percent = 0,
        .top_variable = 156,
        .inverting = false};

    // tick = 4 us
    // 0 degree = 1000 us / 4 us = 250
    // 180 degree = 2000 us / 4 us = 500
    // each degree = (500 - 250) / 180 = 1.389 (as we have 250 us interval and we can increment 1 us, so our precison is 1 degree)
    avr8_timer_1_config_t timer1_cfg = {
        .output = PWM1_OC1A,
        .mode = PWM1_FAST_ICR1,
        .prescaler = 64,
        .duty_percent = 0,
        .top_variable = 4999,
        .inverting = false};

    avr8_timer_2_config_t timer2_cfg = {
        .output = PWM2_OC2A,
        .mode = PWM2_FAST,
        .prescaler = 8,
        .duty_percent = 0,
        .inverting = false};

    avr8_timer_0_init(&timer0_cfg);
    avr8_timer_1_init(&timer1_cfg);
    avr8_timer_2_init(&timer2_cfg);

    uint8_t duty;

    while (1)
    {
        avr8_timer_0_set_duty(&timer0_cfg, 7);   // 0°
        avr8_timer_1_set_duty(&timer1_cfg, 249); // 0°
        _delay_ms(1000);

        avr8_timer_0_set_duty(&timer0_cfg, 8);   // 4°
        avr8_timer_1_set_duty(&timer1_cfg, 255); // 4°
        _delay_ms(1000);

        avr8_timer_0_set_duty(&timer0_cfg, 9);   // 27°
        avr8_timer_1_set_duty(&timer1_cfg, 287); // 90°
        _delay_ms(1000);

        avr8_timer_0_set_duty(&timer0_cfg, 10);  // 50°
        avr8_timer_1_set_duty(&timer1_cfg, 319); // 50°
        _delay_ms(1000);

        avr8_timer_0_set_duty(&timer0_cfg, 11);  // 73°
        avr8_timer_1_set_duty(&timer1_cfg, 351); // 73°
        _delay_ms(1000);

        avr8_timer_0_set_duty(&timer0_cfg, 12);  // 96°
        avr8_timer_1_set_duty(&timer1_cfg, 383); // 96°
        _delay_ms(1000);

        avr8_timer_0_set_duty(&timer0_cfg, 13);  // 119
        avr8_timer_1_set_duty(&timer1_cfg, 415); // 50°
        _delay_ms(1000);

        avr8_timer_0_set_duty(&timer0_cfg, 14);  // 142°
        avr8_timer_1_set_duty(&timer1_cfg, 447); // 142°
        _delay_ms(1000);

        avr8_timer_0_set_duty(&timer0_cfg, 15);  // 165
        avr8_timer_1_set_duty(&timer1_cfg, 479); // 165°
        _delay_ms(1000);

        avr8_timer_0_set_duty(&timer0_cfg, 16);  // 180°
        avr8_timer_1_set_duty(&timer1_cfg, 500); // 180°
        _delay_ms(1000);

        for (duty = 0; duty <= 100; duty++)
        {
            avr8_timer_2_set_duty_percent(&timer2_cfg, duty);
            _delay_ms(10);
        }

        for (duty = 100; duty > 0; duty--)
        {
            avr8_timer_2_set_duty_percent(&timer2_cfg, duty);
            _delay_ms(10);
        }
    }

    return 0;
}