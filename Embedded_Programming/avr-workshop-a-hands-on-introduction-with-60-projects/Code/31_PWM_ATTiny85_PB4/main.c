#include <avr/io.h>
#include <util/delay.h>

void initPWM(void)
{
    // Config the directio of the Port PB4
    DDRB |= (1 << PORTB4);

    // Enable PWM based on comparator OCR1B
    GTCCR |= (1 << PWM1B);
    // Config the timer1 pre-scaler to 1.
    TCCR1 |= (1 << CS10);
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
            OCR1B = duty;
            _delay_ms(10);
        }
        for (duty = 255; duty > 0; --duty)
        {
            // Set Compare amount
            OCR1B = duty;
            _delay_ms(10);
        }
    }
}