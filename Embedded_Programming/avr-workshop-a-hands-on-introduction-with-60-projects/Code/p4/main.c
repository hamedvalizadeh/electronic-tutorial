// Project 3 - Bit-Shifting Digital Outputs

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  DDRB = 0b11111111; // set PORTB register as outputs

  while(1)
  {
	PORTB = 0b00001111;
    _delay_ms(1000);
	PORTB = ~0b00001111;
    _delay_ms(1000);
  }
  return 0;             
}
