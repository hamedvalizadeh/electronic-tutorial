
#include "avr/io.h"
#include <util/delay.h>

void PWM_init()
{
    // Config direction pf port PB1 as output.
    DDRB |= (1 << PB1);

    // Config timer1 mode as FAST PWM 8 bit
    TCCR1A |= (1 << WGM10);
    TCCR1B |= (1 << WGM12);

    // Config the timer1 pre-scalar as 8
    TCCR1B |= (1 << CS11);

    // Config OC1A (connected to port PB1) as non-inverting PWM mode
    TCCR1A |= (1 << COM1A1);
}

int main()
{
    unsigned char duty;

    PWM_init();
    while (1)
    {
        for (duty = 0; duty < 255; duty++)
        {
            // Set comparer register related to OC1A of timer1
            OCR1A = duty;
            _delay_ms(8);
        }
        for (duty = 255; duty > 1; duty--)
        {
            // Set comparer register related to OC1A of timer1
            OCR1A = duty;
            _delay_ms(8);
        }
    }
    return 0;
}