
#include "avr/io.h"
#include <util/delay.h>

void PWM_init()
{
    // Config direction pf port PD6 as output.
    DDRD |= (1 << PD6);

    // Config timer0 mode as FAST PWM
    TCCR0A |= (1 << WGM00) | (1 << WGM01);

    // Config the timer0 pre-scalar as 8
    TCCR0B |= (1 << CS01);

    // Config OC0A (connected to port PD6) as non-inverting PWM mode
    TCCR0A |= (1 << COM0A1);
}

int main()
{
    unsigned char duty;

    PWM_init();
    while (1)
    {
        for (duty = 0; duty < 255; duty++)
        {
            // Set comparer register related to OC0A of timer0
            OCR0A = duty;
            _delay_ms(8);
        }
        for (duty = 255; duty > 1; duty--)
        {
            // Set comparer register related to OC0A of timer0
            OCR0A = duty;
            _delay_ms(8);
        }
    }
    return 0;
}