#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB  = 0b00000001; // set PB4 as INPUT and PB0 as output
	PORTB = 0b00000000; 
    while (1)
    {
      if(PINB == 0b00010000)
	  {
		PORTB = 0b00000001;
        _delay_ms(100);
		//PORTB = 0b00000000;
	  }
	  else
	  {
		PORTB = 0b00000000;
	  }
    }
	return 0;
}
	