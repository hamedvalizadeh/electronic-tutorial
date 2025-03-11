#include "avr/io.h"
#include <util/delay.h>
#include <avr/interrupt.h>

void port_init()
{
    // set the direction of PORTB1 as output to control logic on IN1 pin of L293D
    DDRB |= (1 << PORTB1);
    // set the direction of PORTB2 as output to control logic on IN2 pin of L293D
    DDRB |= (1 << PORTB2);

    // set the direction of PORTB3 as output to control PWM on EN1 pin of L293D
    DDRB |= (1 << PORTB3);
}

void timer1_init()
{
    // Config timer1 mode as 8 bit FAST PWM
    TCCR2A |= (1 << WGM20);
    TCCR2A |= (1 << WGM21);

    // Config the timer1 pre-scalar as 8
    TCCR2B |= (1 << CS21);

    // Config OC1A (connected to port PB1) as non-inverting PWM mode
    TCCR2A |= (1 << COM2A1);
}

void motor_forward()
{
    PORTB |= (1 << PORTB1);
    PORTB &= ~(1 << PORTB2);
}

void motor_backward()
{
    PORTB &= ~(1 << PORTB1);
    PORTB |= (1 << PORTB2);
}

void motor_pwm(uint16_t duty)
{
    // Config OC2A (connected to port PB1) as non-inverting PWM mode
    TCCR2A |= (1 << COM2A1);

    // set vlue for OCR2A (PORTB3) connected to the EN1 pin of L293D to change speed of the motor
    OCR2A = duty;
}

void motor_on()
{
    // Config PORTB3 as normal mode
    TCCR2A &= ~(1 << COM2A1);

    // set PORTB3 connected to the EN1 pin of L293D as high to run motorwith maximum speed
    PORTB |= (1 << PORTB3);
}

void motor_stop()
{
    // Config PORTB3 as normal mode
    TCCR2A &= ~(1 << COM2A1);

    // set PORTB3 connected to the EN1 pin of L293D as low to stop motor
    PORTB &= ~(1 << PORTB3);
}

int main()
{
    port_init();
    timer1_init();

    while (1)
    {
        motor_stop();
        _delay_ms(1000);
        motor_forward();
        motor_pwm(50);
        _delay_ms(2000);
        motor_pwm(150);
        _delay_ms(2000);
        motor_stop();
        _delay_ms(3000);
        motor_backward();
        motor_pwm(150);
        _delay_ms(2000);
        motor_on();
        _delay_ms(3000);
        motor_stop();
        _delay_ms(3000);
    }
    return 0;
}