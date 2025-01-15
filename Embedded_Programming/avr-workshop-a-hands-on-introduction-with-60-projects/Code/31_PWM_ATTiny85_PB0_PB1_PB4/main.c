#include <avr/io.h>
#include <util/delay.h>

void initPWM(void)
{
    // Config the direction of the Ports PB0, PB1, and PB4
    DDRB |= (1 << PORTB4) | (1 << PORTB1) | (1 << PORTB0);

    // set timer0 mode to FAST PWM
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // Enable PWM based on comparator OCR1B
    GTCCR |= (1 << PWM1B);

    // Config the timer0 pre-scaler to 1.
    TCCR0B |= (1 << CS00);
    // Config the timer1 pre-scaler to 1.
    TCCR1 |= (1 << CS10);

    // Config the comparer: toggle 0C0B (connected to PB1) when TCNT0 (timer0 counter)
    //                      reaches OCR0B (target)
    TCCR0A |= (1 << COM0B1);
    // Config the comparer: toggle OC0A (connected to PB0) when TCNT0 (timer0 counter)
    //                      reaches OCR0A (target)
    TCCR0A |= (1 << COM0A1);
    // Config the comparer: toggle OC1B (connected to PB4) when TCNT1 (timer 1 counter)
    //                      reaches OCR1B (target)
    GTCCR |= (1 << COM1B0);
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
            OCR0B = duty;
            OCR1B = duty;
            _delay_ms(10);
        }
        for (duty = 255; duty > 0; --duty)
        {
            // Set Compare amount
            OCR0A = duty;
            OCR0B = duty;
            OCR1B = duty;
            _delay_ms(10);
        }
    }
}