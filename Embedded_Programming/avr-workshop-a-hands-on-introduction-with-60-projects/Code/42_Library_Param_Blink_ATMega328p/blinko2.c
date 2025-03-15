#include <avr/io.h>
#include <util/delay.h>

void blinko2_type(int blink_count, int blink_duration)
{
    uint8_t i;
    for (i = 0; i < blink_count; i++)
    {
        PORTB |= (1 << PORTB0);
        _delay_ms(blink_duration);
        PORTB &= ~(1 << PORTB0);
        _delay_ms(blink_duration);
    }
}