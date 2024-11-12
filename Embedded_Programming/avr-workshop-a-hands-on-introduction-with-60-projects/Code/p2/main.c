// Project 2 - Experimenting with ATmega328P-PU Digital Outputs

#include <avr/io.h>
#include <util/delay.h>
 
int main(void)
{
    DDRB = 0b11111111; // set PORTB register as outputs
    for(;;)
    {
    _delay_ms(250);
    PORTB = 0b11111111;
    _delay_ms(250);     
    PORTB = 0b00000000;
    }
    return 0;             
}
