#include <avr/io.h>
#include <math.h>
#include <util/delay.h>

void setup_adc()
{
    ADMUX  |= (1 << REFS0); //select AVCC
    ADMUX  |= (1 << MUX2) | (1 << MUX0); // select ADC5
    ADCSRA |= (1 << ADEN); //enable ADC
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0); //division ration of ADC speed = 8
}

uint16_t read_adc()
{
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));
    _delay_ms(5);
    return ADC;
}

uint8_t adc_to_temp_celcius(uint16_t adc_val)
{
    float volt = (adc_val * 5);
    volt = volt / 1024;
    float temp = ((volt - 0.5) * 100);
    return ((uint8_t)round(temp));
}

void display_in_7seg(uint8_t num)
{
    switch (num)
    {
        case 0: PORTB = 0b00111111; break;
        case 1: PORTB = 0b00000110; break;
        case 2: PORTB = 0b01011011; break;
        case 3: PORTB = 0b01001111; break;
        case 4: PORTB = 0b01100110; break;
        case 5: PORTB = 0b01101101; break;
        case 6: PORTB = 0b01111101; break;
        case 7: PORTB = 0b00000111; break;
        case 8: PORTB = 0b01111111; break;
        case 9: PORTB = 0b01101111; break;
    }
}

int main(void)
{
    uint16_t adc_val;
    uint8_t temp;
    
    uint8_t tens;
    uint8_t ones;

    DDRB = 0b11111111;
    DDRC = 0b00000000;
        
    setup_adc();

    while(1)
    {
        adc_val = read_adc();
        temp = adc_to_temp_celcius(adc_val);
        tens = temp / 10;
        ones = temp % 10;

        display_in_7seg(tens);
        _delay_ms(250);
        display_in_7seg(ones);
        
        _delay_ms(1000);
    }
    return 0;             
}
