#include <avr/io.h>
#include <util/delay.h>

void initPWM(void)
{
    // Config port PB0 direction
    DDRB |= (1 << PORTB0);

    // set timer0 mode to FAST PWM
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // Config the timer0 pre-scaler to 1.
    TCCR0B |= (1 << CS00);
    // Config the comparer: toggle OC0A (connected to PB0) when TCNT0 (timer0 counter)
    //                      reaches OCR0A (target)
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
            // Set Compare amount
            OCR0A = duty;
            _delay_ms(10);
        }
        for (duty = 255; duty > 0; --duty)
        {
            // Set Compare amount
            OCR0A = duty;
            _delay_ms(10);
        }
    }
}