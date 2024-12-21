#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(TIMER1_COMPA_vect)
{
    // TCNT1 = 0;
    PORTB |= (1 << PORTB1);
    _delay_ms(1000);
    PORTB &= ~(1 << PORTB1);
}

void setup_timer1_ctc_interrupt()
{
    // number of periods to watch for.
    // this is calculated as: OCR1A = round(Seconds / (1 / ( F/Prescalar ) ) ) + 1
    // OCR1A = round( 5 / ( 1 / ( 1000000 / 1024 ) ) ) + 1 = round( 5 / ( 1 / ( 976.5625 ) ) ) + 1
    // = round( 5 / ( 0.001024 ) ) + 1 = round( 4882.8125 ) + 1 = 4884
    OCR1A = 4884;

    // set prescalar to 1024
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    // TCCR1B |= (1 << CS12) | (1 << CS10);

    // enable ctc 1 A interrupt
    TIMSK1 |= (1 << OCIE1A);

    sei();
}

int main(void)
{
    setup_timer1_ctc_interrupt();

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