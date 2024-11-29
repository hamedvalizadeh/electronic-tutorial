#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect)
{
    // set 1 in PB0
    PORTB |= (1 << PORTB0);
    _delay_ms(2000);
    // set 0 in PB0
    PORTB |= (0 << PORTB0);

    EIFR &= ~(1 << 0);
}

ISR(INT1_vect)
{
    // set 1 in PB0
    PORTB |= (1 << PORTB1);
    _delay_ms(1000);
    // set 0 in PB0
    PORTB |= (0 << PORTB1);

    EIFR &= ~(1 << 0);
}

void setup_int0()
{
    // turn on interrupt on pin 4 (INT0, PD2)
    EIMSK |= (1 << INT1) | (1 << INT0);

    // set the trigger on rising edge for INT0
    EICRA |= (1 << ISC01) | (1 << ISC00);

    // set the trigger on falling edge for INT1
    EICRA |= (1 << ISC11);

    // enable global interrupt flag
    sei();

    // set the PD2 as input;
    DDRD &= ~(1 << PORTD2);

    // set the PD3 as input;
    DDRD &= ~(1 << PORTD3);
}

int main(void)
{
    setup_int0();
    // Set PB0 as output
    DDRB |= (1 << PORTB0) | (1 << PORTB1);
    while (1)
    {
        // set 1 in PB0
        PORTB = (0b00000011);
        _delay_ms(50);
        // set 0 in PB0
        PORTB = (0b00000000);
        _delay_ms(50);
    }
    return 0;
}
