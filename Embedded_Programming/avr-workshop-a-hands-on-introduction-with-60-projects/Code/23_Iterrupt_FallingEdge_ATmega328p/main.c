#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect)
{
    // set 1 in PB0
    PORTB |= (1 << PORTB0);
    _delay_ms(2000);
    // set 0 in PB0
    PORTB &= ~(1 << PORTB0);

    EIFR &= ~(1 << 0);
}

void setup_int0()
{
    // turn on interrupt on pin 4 (INT0, PD2)
    EIMSK |= (1 << INT0);

    // st the trigger on rising edge
    EICRA |= (1 << ISC01);

    // enable global interrupt flag
    sei();

    // set the PD2 as input;
    DDRD &= ~(1 << PORTD2);
}

int main(void)
{
    setup_int0();
    // Set PB0 as output
    DDRB |= (1 << PORTB0);
    while (1)
    {
        // set 1 in PB0
        PORTB |= (1 << PORTB0);
        _delay_ms(50);
        // set 0 in PB0
        PORTB &= ~(1 << PORTB0);
        _delay_ms(50);
    }
    return 0;
}
