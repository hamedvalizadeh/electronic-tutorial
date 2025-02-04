#include "avr/io.h"
#include <util/delay.h>

void PWM_init()
{
    // Config direction pf port PB3 as output.
    DDRB |= (1 << PB3);

    // Config timer2 mode as FAST PWM
    TCCR2A |= (1 << WGM20) | (1 << WGM21);

    // Config the timer2 pre-scalar as 8
    TCCR2B |= (1 << CS21);

    // Config OC2A (connected to port PB3) as non-inverting PWM mode
    TCCR2A |= (1 << COM2A1);
}

int main()
{
    unsigned char duty;

    PWM_init();
    while (1)
    {
        for (duty = 0; duty < 255; duty++)
        {
            // Set comparer register related to OC2A of timer2
            OCR2A = duty;
            _delay_ms(8);
        }
        for (duty = 255; duty > 1; duty--)
        {
            // Set comparer register related to OC2A of timer2
            OCR2A = duty;
            _delay_ms(8);
        }
    }
    return 0;
}