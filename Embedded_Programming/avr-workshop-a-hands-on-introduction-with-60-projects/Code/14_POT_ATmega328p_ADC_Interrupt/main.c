#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint16_t adc_val;

// ADC interrupt function
ISR(ADC_vect)
{
	// set off PORTS PB0, PB1, PB2, and PB3.
	PORTB &= ~(1 << PB3) & ~(1 << PB2) & ~(1 << PB1) & ~(1 << PB0);
	adc_val = ADC;
	if (adc_val < 256)
	{
		// set on PORT PB0.
		PORTB |= (1 << PB0);
	}
	else if (adc_val >= 256 && adc_val < 512)
	{
		// set on PORT PB1.
		PORTB |= (1 << PB1);
	}
	else if (adc_val >= 512 && adc_val < 768)
	{
		// set on PORT PB2.
		PORTB |= (1 << PB2);
	}
	else
	{
		// set on PORT PB3.
		PORTB |= (1 << PB3);
	}
	_delay_ms(500);

	// Start conversion again
	adc_start_convertion();
}

void adc_init()
{
	// Select Vref=AVCC
	ADMUX |= (1 << REFS0);

	// Select ADC5 pin (PORT PC5) as ADC input channel
	ADMUX |= (1 << MUX2) | (1 << MUX0);

	// Set PORT PC5 as input to capture ADC value on this pin
	DDRC &= ~(1 << PC5);

	// Set prescaler to 128 (ADC clock = F_CPU/128)
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// Enable ADC
	ADCSRA |= (1 << ADEN);

	// Enable ADC interrupt
	ADCSRA |= (1 << ADIE);
}

void adc_start_convertion()
{
	// Start ADC convertion again
	ADCSRA |= (1 << ADSC);
}

int main(void)
{
	// Set direction of PORTS PB0, PB1, PB2, and PB3 as output.
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

	adc_init();

	// Enable global interrupts
	sei();

	// Start the first conversion
	adc_start_convertion();

	while (1)
	{
	}

	return 0;
}