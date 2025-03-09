#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint16_t adc_val;

//***************** Start: ADC Interrupt *****************

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

void setup_portb_for_adc()
{
	// Set direction of PORTS PB0, PB1, PB2, and PB3 as output.
	DDRB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3);
}

void setup_adc()
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

//***************** End: ADC Interrupt *****************

//***************** Start: Pin Change Interrupt *****************

void setup_portb_for_pci()
{
	// Set PORTB4 as input for Button ON
	DDRB &= ~(1 << PORTB4);
	// Set PORTB5 as input for Button OFF
	DDRB &= ~(1 << PORTB5);

	// Set PORTB6 as output for status LED: status of Button ON
	DDRB |= (1 << PORTB6);
	// Set PORTB7 as output for status LED: status of Button OFF
	DDRB |= (1 << PORTB7);
}

// this function enable pin change interrupt on bank0 for pins PCINT4 and PCINT5:
//  PCINT0: PB0
//  PCINT1: PB1
//  PCINT2: PB2
//  PCINT3: PB3
//  PCINT4: PB4 *
//  PCINT5: PB5 *
//  PCINT6: PB6
//  PCINT7: PB7
void setup_pin_change_interrupt()
{
	// Enable interrupt on bank 0
	PCICR |= (1 << PCIE0);
	// Enable interrupt on PCINT4 (PIN4) of bank 0
	PCMSK0 |= (1 << PCINT4);
	// Enable interrupt on PCINT5 (PIN5) of bank 0
	PCMSK0 |= (1 << PCINT5);
}

ISR(PCINT0_vect)
{
	PORTB &= ~(1 << PORTB6);
	PORTB &= ~(1 << PORTB7);

	// Check if PINB4 is low: Button ON is clicked
	if ((PINB & (1 << PINB4)))
	{
		// set On PORTB6 light up LED on PB6
		PORTB |= (1 << PORTB6);
	}

	// Check if PINB5 is low: Button OFF is clicked
	if ((PINB & (1 << PINB5)))
	{
		// set On PORTB7 light up LED on PB7
		PORTB |= (1 << PORTB7);
	}
	_delay_ms(500);
}
//***************** End: Pin Change Interrupt *****************

int main(void)
{
	setup_portb_for_adc();
	setup_adc();

	setup_portb_for_pci();
	setup_pin_change_interrupt();

	// Enable global interrupts
	sei();

	// Start the first conversion
	adc_start_convertion();

	while (1)
	{
	}

	return 0;
}