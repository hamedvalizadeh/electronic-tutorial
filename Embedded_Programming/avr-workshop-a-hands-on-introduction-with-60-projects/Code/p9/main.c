#include <avr/io.h>
#include <util/delay.h>

void blinkTwice()
{
	PORTB = 0b11111111;
	_delay_ms(100);
	
	PORTB = 0b00000000;
	_delay_ms(100);
	
	PORTB = 0b11111111;
	_delay_ms(100);
	
	PORTB = 0b00000000;
	_delay_ms(100);
}

int main(void)
{
    DDRB = 0b11111111; // set PB4 as INPUT and PB0 as output
	for(;;)
	{
		blinkTwice();
		_delay_ms(5000);
	}
	return 0;
}