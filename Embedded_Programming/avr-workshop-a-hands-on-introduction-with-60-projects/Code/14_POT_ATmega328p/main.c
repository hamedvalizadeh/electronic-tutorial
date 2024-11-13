#include <avr/io.h> 
#include <math.h>
#include <util/delay.h>

void setup_adc()
{
	ADMUX  |= (1 << REFS0); //select AVCC (pin 20 of the ATMega328p) as the voltage source for ADC circuit. 
	ADMUX  |= (1 << MUX2) | (1 << MUX0); //select ADC5 (pin 28 of the ATMega328p)
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t read_value_adc()
{
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & ((1 << ADSC)));
	_delay_ms(10);
	return ADC;
}

int main(void)
{
	uint16_t adc_val;
	DDRB = 0b00001111;
	DDRC = 0b00000000;
	setup_adc();
	while (1)
	{
		adc_val = read_value_adc();
		if(adc_val < 256)
		{
			PORTB = 0b00000001;
		}
		else if(adc_val >= 256 && adc_val < 512)
		{
			PORTB = 0b00000010;
		}
		else if(adc_val >= 512 && adc_val < 768)
		{
			PORTB = 0b00000100;
		}
		else
		{
			PORTB = 0b00001000;
		}
		_delay_ms(100);
	}
	
	return 0;
}