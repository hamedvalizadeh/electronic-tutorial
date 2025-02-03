#include <avr/interrupt.h>

ISR(TIMER2_OVF_vect)
{
    // Toggle the logic of PORT PB1
    PORTB ^= (1 << PB1);
}

void init_timer2()
{
    // Config direction of port PB1 as output.
    DDRB |= (1 << PB1);

    // Config the timer2 pre-scalar as 128
    TCCR2B |= (1 << CS22) | (1 << CS20);

    // Config the clock source of timer2 as external clock (we use crystal clock with 32678Hz frequency)
    ASSR |= (1 << AS2);

    // enable timer2 counter overflow 2 interrupt
    TIMSK2 |= (1 << TOIE2);

    // enable global interrupt flag
    sei();
}

int main()
{
    init_timer2();
    while (1)
    {
    }
    return 0;
}