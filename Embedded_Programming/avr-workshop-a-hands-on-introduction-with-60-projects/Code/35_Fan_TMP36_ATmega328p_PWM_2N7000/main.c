#include "avr/io.h"
#include <util/delay.h>
#include <avr/interrupt.h>

float temperature;

float adc_to_temp_celcius(uint16_t adc_val)
{
    float volt = (adc_val * 5);
    volt = volt / 1024;
    float temp = ((volt - 0.5) * 100);
    return temp;
}

void port_init()
{
    // set PORT PC5 direction as input. to capture the analoge value of TMP36.
    DDRC &= ~(1 << PORTC5);

    // set PORT PB1 direction as output. to control speed of DC-FAN.
    DDRB |= (1 << PORTB1);

    DDRB |= (1 << PORTB2);
    DDRB |= (1 << PORTB3);
    DDRB |= (1 << PORTB4);
    DDRB |= (1 << PORTB5);
}

void adc_setup()
{
    // select AVCC as external voltage source for ADC
    ADMUX |= (1 << REFS0);

    // select ADC5 channel on PORT PC5
    ADMUX |= (1 << MUX2) | (1 << MUX0);

    // Enable ADC
    ADCSRA |= (1 << ADEN);

    // set devision factor to 8
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0);

    // Enable ADC Interrupt
    ADCSRA |= (1 << ADIE);
}

void adc_start_conversion()
{
    // start ADC conversion
    ADCSRA |= (1 << ADSC);
}

ISR(ADC_vect)
{
    temperature = adc_to_temp_celcius(ADC);

    PORTB &= ~(1 << PB5) & ~(1 << PB4) & ~(1 << PB3) & ~(1 << PB2);

    if (temperature < 25)
    {
        PORTB |= (1 << PB2);
        motor_off();
    }
    else if (temperature >= 25 && temperature < 35)
    {
        PORTB |= (1 << PB3);
        motor_pwm(50);
    }
    else if (temperature >= 35 && temperature < 45)
    {
        PORTB |= (1 << PB4);
        motor_pwm(200);
    }
    else
    {
        PORTB |= (1 << PB5);
        motor_on();
    }
    _delay_ms(500);
    adc_start_conversion();
}

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

int main()
{
    port_init();
    adc_setup();
    timer1_init();

    sei();
    adc_start_conversion();

    while (1)
    {
    }
    return 0;
}