#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void setup_timer1_interrupt()
{
    // set prescalar to 8
    TCCR1B |= (1 << CS11);

    // enable timer counter overflow 0 interrupt
    TIMSK1 |= (1 << TOIE0);

    sei();
}

ISR(TIMER1_OVF_vect)
{
    PORTB |= (1 << PORTB1);
    _delay_ms(100);
    PORTB &= ~(1 << PORTB1);
}

int main(void)
{
    setup_timer1_interrupt();
    DDRB |= (1 << PORTB0);
    DDRB |= (1 << PORTB1);
    while (1)
    {
        PORTB |= (1 << PORTB0);
        _delay_ms(100);
        PORTB &= ~(1 << PORTB0);
        _delay_ms(100);
    };
    return 0;
}