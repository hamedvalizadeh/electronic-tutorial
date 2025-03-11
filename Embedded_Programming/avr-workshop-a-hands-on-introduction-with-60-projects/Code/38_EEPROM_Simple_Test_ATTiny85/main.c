#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

int main(void)
{
    DDRB |= (1 << PORTB0);
    DDRB |= (1 << PORTB1);
    DDRB |= (1 << PORTB2);
    DDRB |= (1 << PORTB3);

    int i = 0;
    while (1)
    {
        for (i = 0; i < 16; i++)
        {
            eeprom_update_byte((uint8_t *)i, i);
        }
        for (i = 0; i < 16; i++)
        {
            PORTB = eeprom_read_byte((uint8_t *)i);
            _delay_ms(1000);
        }
        PORTB &= ~(1 << PORTB) & ~(1 << PORTB2) & ~(1 << PORTB1) & ~(1 << PORTB0);
    }
    return 0;
}
