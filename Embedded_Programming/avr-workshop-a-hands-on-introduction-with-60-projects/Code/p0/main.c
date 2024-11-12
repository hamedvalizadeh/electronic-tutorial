// Project 0 - Blinking an LED

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB = 0b00001000; // set PB3 as output
    while (1)
    {
        //PORTB = 0b11111111;      
		PORTB = 0b00001000; 
        _delay_ms(1000);
        //PORTB = 0b00000000;            
		PORTB = 0b00000000;
        _delay_ms(1000);
    }
return 0;
}
	