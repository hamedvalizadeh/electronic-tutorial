#include "avr/io.h"
#include <util/delay.h>

#define wait 8

void port_init()
{
    // Config direction of ports PB1, PB2, and PB3 as output.
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
}

void timer1_init()
{
    // Config timer1 mode as 8 bit FAST PWM
    TCCR1A |= (1 << WGM12) | (1 << WGM10);

    // Config the timer1 pre-scalar as 8
    TCCR1B |= (1 << CS11);

    // Config OC1A (connected to port PB1) as non-inverting PWM mode
    TCCR1A |= (1 << COM1A1);

    // Config OC1B (connected to port PB2) as non-inverting PWM mode
    TCCR1A |= (1 << COM1B1);
}

void timer2_init()
{
    // Config timer2 mode as FAST PWM
    TCCR2A |= (1 << WGM21) | (1 << WGM20);

    // Config the timer2 pre-scalar as 8
    TCCR2B |= (1 << CS21);

    // Config OC2A (connected to port PB3) as non-inverting PWM mode
    TCCR2A |= (1 << COM2A1);
}

void blue_pwm(uint8_t duty)
{
    // Set comparer register related to OC1A of timer1
    OCR1A = duty;
}

void green_pwm(uint8_t duty)
{
    // Set comparer register related to OC1B of timer1
    OCR1B = duty;
}

void red_pwm(uint8_t duty)
{
    // Set comparer register related to OC2A of timer2
    OCR2A = duty;
}

int main()
{
    port_init();
    timer1_init();
    timer2_init();

    uint8_t duty;

    while (1)
    {
        blue_pwm(0);
        green_pwm(0);
        red_pwm(0);

        // Red to Green
        for (duty = 0; duty < 255; duty++)
        {
            red_pwm(255 - duty);
            green_pwm(duty);
            _delay_ms(wait);
        }

        // Green to Blue
        for (duty = 255; duty > 1; duty--)
        {
            green_pwm(255 - duty);
            blue_pwm(duty);
            _delay_ms(wait);
        }
        
        // Blue to Red
        for (duty = 255; duty > 1; duty--)
        {
            blue_pwm(255 - duty);
            red_pwm(duty);
            _delay_ms(wait);
        }
    }
    return 0;
}