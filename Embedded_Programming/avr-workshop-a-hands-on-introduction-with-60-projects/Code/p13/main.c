#include <avr/io.h>
#include <util/delay.h>


void set_ADC()
{
    ADMUX  = 0b00100010;
    ADCSRA = 0b10000011;
}

int main(void)
{
    DDRB = 0b00000011;
    set_ADC();
    while (1)
    {
        ADCSRA |= (1 << ADSC);
        while(ADCSRA&(1 << ADSC));
        _delay_ms(5);
        double volt_value = (double)((((int)ADCH) * 5) / 256);
        if(volt_value >= 1.4)
        {
            PORTB = 0b00000010;
        }
        if(volt_value < 1.4)
        {
            PORTB = 0b00000001;
        }
    }    
    return 0;
}
