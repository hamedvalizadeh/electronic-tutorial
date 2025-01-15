#include <avr/io.h>
#include <util/delay.h>

void initPWM(void)
{
    DDRB |= (1 << PORTB0); // set PB4, PB1, PB0 as outputs

    // set timer mode to FAST PWMF
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // no prescaler
    TCCR0B |= (1 << CS00);
    // connect PWM signal to pin (OC0A => PB0)
    TCCR0A |= (1 << COM0A1);
}

int main(void)
{
    uint8_t duty = 0;

    initPWM();

    while (1)
    {
        for (duty = 1; duty < 255; duty++)
        {
            OCR0A = duty; // PB0
            _delay_ms(10);
        }
        for (duty = 255; duty > 0; --duty)
        {
            OCR0A = duty; // PB0
            _delay_ms(10);
        }
    }
}