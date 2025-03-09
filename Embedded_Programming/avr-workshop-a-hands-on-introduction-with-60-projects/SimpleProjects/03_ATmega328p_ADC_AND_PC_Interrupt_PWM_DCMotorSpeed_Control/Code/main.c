#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint16_t adc_val;
uint8_t is_on = 0;
uint8_t is_off = 1;
uint8_t is_pwm = 0;

void port_init()
{
	// Config direction of ports PB1 as output,to control Motor speed by changing PWM on it.
	DDRB |= (1 << PORTB1);

	// Set PORTB as input for Button ON
	DDRB &= ~(1 << PORTB2);
	// Set PORTB3 as input for Button OFF
	DDRB &= ~(1 << PORTB3);
	// Set PORTB4 as input for Button PWM
	DDRB &= ~(1 << PORTB4);

	// Set PORTB5 as output for status LED: status of Button ON
	DDRB |= (1 << PORTB5);
	// Set PORTB6 as output for status LED: status of Button OFF
	DDRB |= (1 << PORTB6);
	// Set PORTB7 as output for status LED: status of Button PWM
	DDRB |= (1 << PORTB7);
}

//***************** Start: ADC Interrupt *****************

// ADC interrupt function
ISR(ADC_vect)
{
	if (is_off == 1)
	{
		adc_val = 0;
	}
	else if (is_on == 1)
	{
		adc_val = 0;
	}
	else if(is_pwm == 1)
	{
		adc_val = ADC;
		motor_pwm(adc_val);
	}

	_delay_ms(500);
	
	// Start conversion again
	adc_start_convertion();
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

// this function enable pin change interrupt on bank0 for pins PCINT4 and PCINT5:
//  PCINT0: PB0
//  PCINT1: PB1
//  PCINT2: PB2 *
//  PCINT3: PB3 *
//  PCINT4: PB4 *
//  PCINT5: PB5 
//  PCINT6: PB6
//  PCINT7: PB7
void setup_pin_change_interrupt()
{
	// Enable interrupt on bank 0
	PCICR |= (1 << PCIE0);
	// Enable interrupt on PCINT2 (PINB2) of bank 0
	PCMSK0 |= (1 << PCINT2);
	// Enable interrupt on PCINT3 (PINB3) of bank 0
	PCMSK0 |= (1 << PCINT3);
	// Enable interrupt on PCINT4 (PINB4) of bank 0
	PCMSK0 |= (1 << PCINT4);
}

ISR(PCINT0_vect)
{
	PORTB &= ~(1 << PORTB5);
	PORTB &= ~(1 << PORTB6);
	PORTB &= ~(1 << PORTB7);

	// Check if PINB3 is low: Button ON is clicked
	if ((PINB & (1 << PINB2)))
	{
		is_on = 1;
		is_off = 0;
		is_pwm = 0;
		// set On PORTB5 light up LED on PB5
		PORTB |= (1 << PORTB5);
		motor_on();
	}
	// Check if PINB4 is low: Button OFF is clicked
	else if ((PINB & (1 << PINB3)))
	{
		is_on = 0;
		is_off = 1;
		is_pwm = 0;
		// set On PORTB6 light up LED on PB6
		PORTB |= (1 << PORTB6);
		motor_off();
	}
	// Check if PINB4 is low: Button OFF is clicked
	else if ((PINB & (1 << PINB4)))
	{
		is_on = 0;
		is_off = 0;
		is_pwm = 1;
		// set On PORTB7 light up LED on PB7
		PORTB |= (1 << PORTB7);
	}
	_delay_ms(500);
}

//***************** End: Pin Change Interrupt *****************

//***************** Start: PWM *****************

void timer1_init()
{
	// Config timer1 mode as 8 bit FAST PWM
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1 << WGM12);

	// Config the timer1 pre-scalar as 8
	TCCR1B |= (1 << CS11);

	// Config OC1A (connected to port PB1) as non-inverting PWM mode
	TCCR1A |= (1 << COM1A1);
}

void motor_off()
{
	TCCR1A &= ~(1 << COM1A1);
	PORTB &= ~(1 << PORTB1);
}

void motor_on()
{
	TCCR1A &= ~(1 << COM1A1);
	PORTB |= (1 << PORTB1);
}

void motor_pwm(uint8_t duty)
{
	// Config OC1A (connected to port PB1) as non-inverting PWM mode
	TCCR1A |= (1 << COM1A1);
	// Set comparer register related to OC1A of timer1
	OCR1A = duty;
}

//***************** End: PWM *****************

int main(void)
{
	port_init();
	timer1_init();
	setup_adc();
	setup_pin_change_interrupt();

	// Enable global interrupts
	sei();

	// Start the first conversion
	adc_start_convertion();

	motor_off();

	while (1)
	{
	}

	return 0;
}