#include <avr/io.h>
#include <util/delay.h>
#include "avr8_timer.h"
#include "servo_linear.h"

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

    servo_cfg_t servo0_cfg = {
        .us_min = 1000,
        .us_max = 2000,
        .tick_us = avr8_timer_0_get_tick_us(&timer0_cfg),
        .mode_factor = avr8_timer_0_get_mode_factor(&timer0_cfg),
        .speed_deg = 60,
        .speed_time_ms = 100};

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

    servo_cfg_t servo1_cfg = {
        .us_min = 1000,
        .us_max = 2000,
        .tick_us = avr8_timer_1_get_tick_us(&timer1_cfg),
        .mode_factor = avr8_timer_1_get_mode_factor(&timer1_cfg),
        .speed_deg = 60,
        .speed_time_ms = 100};

    avr8_timer_2_config_t timer2_cfg = {
        .output = PWM2_OC2A,
        .mode = PWM2_FAST,
        .prescaler = 8,
        .duty_percent = 0,
        .inverting = false};

    avr8_timer_0_init(&timer0_cfg);
    avr8_timer_1_init(&timer1_cfg);
    avr8_timer_2_init(&timer2_cfg);

    uint8_t i;
    uint8_t duty;

    while (1)
    {
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 0));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));

        for (i = 0; i < 5; i++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, i));
            _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
            _delay_ms(20);
        }
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 5));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));
        _delay_ms(500);

        for (i = 5; i < 28; i++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, i));
            _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
            _delay_ms(20);
        }
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 28));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));
        _delay_ms(500);

        for (i = 28; i < 51; i++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, i));
            _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
            _delay_ms(20);
        }
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 51));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));
        _delay_ms(500);

        for (i = 51; i < 74; i++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, i));
            _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
            _delay_ms(20);
        }
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 74));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));
        _delay_ms(500);

        for (i = 74; i < 97; i++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, i));
            _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
            _delay_ms(20);
        }
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 97));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));
        _delay_ms(500);

        for (i = 97; i < 120; i++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, i));
            _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
            _delay_ms(20);
        }
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 120));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));
        _delay_ms(500);

        for (i = 120; i < 143; i++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, i));
            _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
            _delay_ms(20);
        }
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 143));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));
        _delay_ms(500);

        for (i = 143; i < 166; i++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, i));
            _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
            _delay_ms(20);
        }
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 166));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));
        _delay_ms(500);

        for (i = 166; i < 180; i++)
        {
            avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, i));
            _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
            _delay_ms(20);
        }
        avr8_timer_0_set_pulse_ticks(&timer0_cfg, servo_angle_pulse_ticks(&servo0_cfg, 189));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo0_cfg));
        _delay_ms(500);

        avr8_timer_1_set_pulse_ticks(&timer1_cfg, servo_angle_pulse_ticks(&servo1_cfg, 180));
        _delay_ms(servo_get_rotation_wait_time_ms(&servo1_cfg));
        _delay_ms(500);

        for (duty = 0; duty <= 100; duty++)
        {
            avr8_timer_2_set_duty(&timer2_cfg, duty);
            _delay_ms(10);
        }

        for (duty = 100; duty > 0; duty--)
        {
            avr8_timer_2_set_duty(&timer2_cfg, duty);
            _delay_ms(10);
        }
    }

    return 0;
}