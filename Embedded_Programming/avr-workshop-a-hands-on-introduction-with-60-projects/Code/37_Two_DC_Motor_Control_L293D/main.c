#include "avr/io.h"
#include <util/delay.h>

void port_init()
{
    // set the direction of PORTB1 as output to control logic on IN1 pin of L293D
    DDRB |= (1 << PORTB1);
    // set the direction of PORTB2 as output to control logic on IN2 pin of L293D
    DDRB |= (1 << PORTB2);

    // set the direction of PORTB3 (OC2A) as output to control PWM on EN1 pin of L293D
    DDRB |= (1 << PORTB3);

    // set the direction of PORTB4 as output to control logic on IN3 pin of L293D
    DDRB |= (1 << PORTB4);
    // set the direction of PORTB5 as output to control logic on IN4 pin of L293D
    DDRB |= (1 << PORTB5);

    // set the direction of PORTD3 (OC2B) as output to control PWM on EN2 pin of L293D
    DDRD |= (1 << PORTD3);
}

void timer2_init()
{
    // Config timer1 mode as 8 bit FAST PWM
    TCCR2A |= (1 << WGM20);
    TCCR2A |= (1 << WGM21);

    // Config the timer1 pre-scalar as 8
    TCCR2B |= (1 << CS21);

    // Config OC2A (connected to port PB3) as non-inverting Fast PWM mode
    TCCR2A |= (1 << COM2A1);

    // Config OC2B (connected to port PD3) as non-inverting Fast PWM mode
    TCCR2A |= (1 << COM2B1);
}

void motor_forward()
{
    PORTB |= (1 << PORTB1);
    PORTB &= ~(1 << PORTB2);

    PORTB |= (1 << PORTB4);
    PORTB &= ~(1 << PORTB5);
}

void motor_backward()
{
    PORTB &= ~(1 << PORTB1);
    PORTB |= (1 << PORTB2);

    PORTB &= ~(1 << PORTB4);
    PORTB |= (1 << PORTB5);
}

void motor_pwm(uint16_t duty)
{
    // Config OC2A (connected to port PB3) as non-inverting FAST PWM mode
    TCCR2A |= (1 << COM2A1);

    // set vlue for OCR2A (PORTB3) connected to the EN1 pin of L293D to change speed of the motor
    OCR2A = duty;

    // Config OC2B (connected to port PBD) as non-inverting FAST PWM mode
    TCCR2A |= (1 << COM2B1);

    // set vlue for OCR2B (PORTD3) connected to the EN2 pin of L293D to change speed of the motor
    OCR2B = duty;
}

void motor_on()
{
    // Config PORTB3 as normal mode
    TCCR2A &= ~(1 << COM2A1);

    // set PORTB3 connected to the EN1 pin of L293D as high to run motorwith maximum speed
    PORTB |= (1 << PORTB3);

    // Config PORTD3 as normal mode
    TCCR2A &= ~(1 << COM2B1);

    // set PORTD3 connected to the EN2 pin of L293D as high to run motorwith maximum speed
    PORTD |= (1 << PORTD3);
}

void motor_stop()
{
    // Config PORTB3 as normal mode
    TCCR2A &= ~(1 << COM2A1);

    // set PORTB3 connected to the EN1 pin of L293D as low to stop motor
    PORTB &= ~(1 << PORTB3);

    // Config PORTD3 as normal mode
    TCCR2A &= ~(1 << COM2B1);

    // set PORTD3 connected to the EN2 pin of L293D as low to stop motor
    PORTD &= ~(1 << PORTD3);
}

int main()
{
    port_init();
    timer2_init();

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