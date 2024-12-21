#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(PCINT0_vect)
{
    // Check if PINB0 is low
    if (!(PINB & (1 << PINB0)))
    {
        // On and Off PORTB3 to toggle LED connected to it for 1 second.
        PORTB |= (1 << PORTB3);
        _delay_ms(500);
        PORTB &= ~(1 << PORTB3);
        _delay_ms(500);
    }

    // Check if PINB1 is low
    if (!(PINB & (1 << PINB1)))
    {
        // On and Off PORTB4 to toggle LED connected to it for 1 second.
        PORTB |= (1 << PORTB4);
        _delay_ms(500);
        PORTB &= ~(1 << PORTB4);
        _delay_ms(500);
    }
}

void setup_pin_change_bank_0()
{
    // Set PORTB0 as input
    DDRB &= ~(1 << PORTB0);
    // Set PORTB1 as input
    DDRB &= ~(1 << PORTB1);

    // Enable interrupt on bank 0
    PCICR |= (1 << PCIE0);
    // Enable interrupt on PCINT0 of bank 0
    PCMSK0 |= (1 << PCINT0);
    // Enable interrupt on PCINT1 of bank 0
    PCMSK0 |= (1 << PCINT1);

    // Enable globale interrupt
    sei();
}

int main(void)
{
    setup_pin_change_bank_0();

    // Set PORTB2 as output
    DDRB |= (1 << PORTB2);
    // Set PORTB3 as output
    DDRB |= (1 << PORTB3);
    // Set PORTB3 as output
    DDRB |= (1 << PORTB4);

    // On and Off PORTB2 to toggle LED connected to it every 200 mili-seconds.
    while (1)
    {
        PORTB |= (1 << PORTB2);
        _delay_ms(100);
        PORTB &= ~(1 << PORTB2);
        _delay_ms(100);
    }

    return 0;
}