// Project 3 - Bit-Shifting Digital Outputs

#include <avr/io.h>
#include <util/delay.h>

#define TIME 5

int main(void)
{
  DDRB = 0b11111111; // set PORTB register as outputs
  uint8_t i;
  
  while(1)
  {
	for(i = 0; i < 256; i++)
    {
	  PORTB = 0b11111111 & i;
      _delay_ms(TIME);  
    }
  }
  return 0;             
}
