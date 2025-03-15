#include <avr/io.h>
#include <util/delay.h>
#include "blanko.h"

int main(void)
{
    DDRB |= (1 << PORTB0);
    while (1)
    {
        blank_slow();
        _delay_ms(1000);
        blank_fast();
        _delay_ms(1000);
    }
    return 0;
}