#include <avr/io.h>
#include <util/delay.h>

void my_delay(int ms)
{
	uint8_t i;
	for (i = 0; i < ms; i++)
	{
		_delay_ms(1);
	}	
}

void my_blink(int blinks)
{
	uint8_t i;
	for (i = 0; i < blinks; i++)
	{
		PORTB = 0b11111111;
		my_delay(100);
		PORTB = 0b00000000;
		my_delay(100);
	}	
}

int main(void)
{
    DDRB = 0b11111111; // set PB4 as INPUT and PB0 as output
	for(;;)
	{
		my_blink(10);
		_delay_ms(3000);
	}
	return 0;
}