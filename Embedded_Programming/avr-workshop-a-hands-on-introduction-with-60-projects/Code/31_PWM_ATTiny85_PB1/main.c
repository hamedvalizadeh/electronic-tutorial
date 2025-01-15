#include <avr/io.h>
#include <util/delay.h>

void initPWM(void)
{
    // Config port PB1 direction
    DDRB |= (1 << PORTB1);

    // set timer0 mode to FAST PWM
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // Config the timer0 pre-scaler to 1.
    TCCR0B |= (1 << CS00);
    // PB1 0C0B
    // Config the comparer: toggle 0C0B (connected to PB1) when TCNT0 (timer0 counter)
    //                      reaches OCR0B (target)
    TCCR0A |= (1 << COM0B1);
}

int main(void)
{
    uint8_t duty = 0;

    initPWM();

    while (1)
    {
        for (duty = 1; duty < 255; duty++)
        {
            // Set Compare amount
            OCR0B = duty;
            _delay_ms(10);
        }
        for (duty = 255; duty > 0; --duty)
        {
            // Set Compare amount
            OCR0B = duty;
            _delay_ms(10);
        }
    }
}