#include "avr/io.h"
#include <util/delay.h>

#define wait 100

void port_init()
{
    // Config direction of ports PB1, PB2, and PB3 as output.
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
}

void timer1_init()
{
    // Config timer1 mode as 8 bit FAST PWM
    TCCR1A |= (1 << WGM10);
    TCCR1B |= (1 << WGM12);

    // Config the timer1 pre-scalar as 8
    TCCR1B |= (1 << CS11);

    // Config OC1A (connected to port PB1) as non-inverting PWM mode
    TCCR1A |= (1 << COM1A1);
}

void motor_off()
{
    TCCR1A &= ~(1 << COM1A1);
    PORTB &= ~(1 << PB1);
}

void motor_on()
{
    TCCR1A &= ~(1 << COM1A1);
    PORTB |= (1 << PB1);
}

void motor_pwm(uint8_t duty)
{
    // Config OC1A (connected to port PB1) as non-inverting PWM mode
    TCCR1A |= (1 << COM1A1);
    // Set comparer register related to OC1A of timer1
    OCR1A = duty;
}

int main()
{
    port_init();
    timer1_init();

    uint8_t duty;

    while (1)
    {
        motor_off();
        _delay_ms(3000);

        for (duty = 0; duty < 255; duty++)
        {
            motor_pwm(duty);
            _delay_ms(wait);
        }
        
        motor_on();
        _delay_ms(3000);

        for (duty = 254; duty > 0; duty--)
        {
            motor_pwm(duty);
            _delay_ms(wait);
        }
    }
    return 0;
}