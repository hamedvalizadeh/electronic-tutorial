// Project 3 - Bit-Shifting Digital Outputs

#include <avr/io.h>
#include <util/delay.h>

#define TIME 100    // delay in milliseconds

int main(void)
{
  uint8_t i;         // 8-bit integer variable “i”
  DDRB = 0b11111111; // set PORTB register as outputs

  while(1)
  {
    for (i = 0; i < 8; i++)
    {
      _delay_ms(TIME);
      PORTB = 0b00000001 << i;
    }

    for (i = 1; i < 7; i++)
    {
      _delay_ms(TIME);
      PORTB = 0b10000000 >> i;
    }
  }
  return 0;             
}
