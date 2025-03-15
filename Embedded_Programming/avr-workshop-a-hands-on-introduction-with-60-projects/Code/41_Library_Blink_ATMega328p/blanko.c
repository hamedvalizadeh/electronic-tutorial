#include <avr/io.h>
#include <util/delay.h>

void blank_slow()
{
    uint8_t i;
    for (i = 0; i < 5; i++)
    {
        PORTB |= (1 << PORTB0);
        _delay_ms(1000);
        PORTB &= ~(1 << PORTB0);
        _delay_ms(1000);
    }
}

void blank_fast()
{
    uint8_t i;
    for (i = 0; i < 5; i++)
    {
        PORTB |= (1 << PORTB0);
        _delay_ms(250);
        PORTB &= ~(1 << PORTB0);
        _delay_ms(250);
    }
}