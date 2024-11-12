#include <avr/io.h>
#include <util/delay.h>

uint8_t times_three(int subject)
{
	uint8_t product;
	product = subject * 3;
	return product;
}

void my_blink(int blinks)
{
	uint8_t i;
	for (i = 0; i < blinks; i++)
	{
		PORTB = 0b11111111;
		_delay_ms(100);
		PORTB = 0b00000000;
		_delay_ms(100);
	}
}

int main(void)
{
    DDRB = 0b11111111; // set PB4 as INPUT and PB0 as output
	uint8_t j;
	uint8_t k;

	for(;;)
	{
		j = 4;
		k = times_three(j);
		my_blink(k);
		_delay_ms(2000);
	}
	return 0;
}