#include <avr/io.h>
#include <util/delay.h>
#include "blinko2.h"

int main(void)
{
    DDRB |= (1 << PORTB0);
    while (1)
    {
        blinko2_type(10, 150);
        _delay_ms(1000);
        blinko2_type(4, 1000);
        _delay_ms(1000);
    }
    return 0;
}