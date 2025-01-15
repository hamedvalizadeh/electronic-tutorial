// Project 31 - PWM demonstration on ATtiny85

#include <avr/io.h>
#include <util/delay.h>

void initPWM(void)
{
    DDRB |= (1 << PORTB4) | (1 << PORTB1) | (1 << PORTB0); // set PB4, PB1, PB0 as outputs

    // PB0 0C0A
    TCCR0A |= (1 << WGM01) | (1 << WGM00); // set timer mode to FAST PWM
    TCCR0A |= (1 << COM0A1);               // connect PWM signal to pin (OC0A => PB0)
    TCCR0B |= (1 << CS00);                 // no prescaler

    // PB1 0C0B
    TCCR0A |= (1 << COM0B1); // connect PWM signal to pin (OC0B => PB0)

    // PB4 OC1B
    TCCR1 |= (1 << PWM1A) | (1 << COM1A0); // connect PWM signal to pin (OCR0B => PB4)
    GTCCR |= (1 << COM1B0);                // toggle PB4 when when timer reaches OCR1B (target)
    GTCCR |= (1 << PWM1B);                 // clear PB4 when when timer reaches OCR1C (top)
    TCCR1 |= (1 << CS10);                  // no prescaler
}

int main(void)
{
    uint8_t duty = 0;

    initPWM();

    while (1)
    {
        for (duty = 1; duty < 100; duty++)
        {
            OCR0A = duty; // PB0
            OCR0B = duty; // PB1
            OCR1B = duty; // PB4
            _delay_ms(10);
        }
        for (duty = 100; duty > 0; --duty)
        {
            OCR0A = duty; // PB0
            OCR0B = duty; // PB1
            OCR1B = duty; // PB4
            _delay_ms(10);
        }
    }
}