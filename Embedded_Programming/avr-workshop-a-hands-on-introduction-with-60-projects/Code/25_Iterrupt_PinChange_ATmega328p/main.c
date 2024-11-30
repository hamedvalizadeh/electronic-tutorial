#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Interrupt function for bank 0
ISR(PCINT0_vect)
{
    // set 1 in PB0
    PORTB |= (1 << PORTB0);
    _delay_ms(1000);
    // set 0 in PB0
    PORTB &= ~(1 << PORTB0);

    PCIFR &= ~(1 << 0);
}

// Interrupt function for bank 1
ISR(PCINT1_vect)
{
    // set 1 in PB0
    PORTB |= (1 << PORTB0);
    _delay_ms(2000);
    // set 0 in PB0
    PORTB &= ~(1 << PORTB0);

    PCIFR &= ~(1 << 1);
}

// Interrupt function for bank 2
ISR(PCINT2_vect)
{
    // set 1 in PB0
    PORTB |= (1 << PORTB0);
    _delay_ms(3000);
    // set 0 in PB0
    PORTB &= ~(1 << PORTB0);

    PCIFR &= ~(1 << 2);
}

void setup_interrupt()
{
    // Enable interrupt on all banks; bank 2, bank 1, and bank 0
    PCICR |= (1 << PCIE2) | (1 << PCIE1) | (1 << PCIE0);

    // Enable PCINT1 in bank 0
    PCMSK0 |= (1 << PCINT1);

    // Enable PCINT1 in bank 1
    PCMSK1 |= (1 << PCINT8);

    // Enable PCINT1 in bank 2
    PCMSK2 |= (1 << PCINT23);

    // enable global interrupt flag
    sei();

    // set the PB1 as input;
    DDRB &= ~(1 << PORTB1);

    // set the PC0 as input;
    DDRC &= ~(1 << PORTC0);

    // set the PD7 as input;
    DDRD &= ~(1 << PORTD7);
}

void setup_portb()
{
    DDRB |= (1 << PORTB0);
}

int main(void)
{
    setup_interrupt();

    setup_portb();

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
