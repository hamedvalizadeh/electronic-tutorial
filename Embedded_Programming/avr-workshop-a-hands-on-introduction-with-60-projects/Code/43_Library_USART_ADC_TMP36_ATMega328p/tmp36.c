#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

void setup_port_tmp()
{
    DDRB |= (1 << PORTB0);
    DDRB |= (1 << PORTB1);
    DDRB |= (1 << PORTB2);
    DDRB |= (1 << PORTB3);
    DDRB |= (1 << PORTB4);
    DDRB |= (1 << PORTB5);
    DDRB |= (1 << PORTB6);
    DDRB |= (1 << PORTB7);
}

float read_tmp36_in_celcius(uint16_t adc_val)
{
    float volt = (adc_val * 5);
    volt = volt / 1024;
    float temp = ((volt - 0.5) * 100);
    return temp;
}

void print_digit(uint8_t value)
{
    uint8_t tens = (value / 10);
    uint8_t ones = (value % 10);
    uint8_t delayTime = 250;

    switch (tens)
    {
    case 0:
        PORTB = 0b00111111;
        break; // 0
    case 1:
        PORTB = 0b00000110;
        break; // 1
    case 2:
        PORTB = 0b01011011;
        break; // 2
    case 3:
        PORTB = 0b01001111;
        break; // 3
    case 4:
        PORTB = 0b01100110;
        break; // 4
    case 5:
        PORTB = 0b01101101;
        break; // 5
    case 6:
        PORTB = 0b01111101;
        break; // 6
    case 7:
        PORTB = 0b00000111;
        break; // 7
    case 8:
        PORTB = 0b01111111;
        break; // 8
    case 9:
        PORTB = 0b01101111;
        break; // 9
    }

    _delay_ms(delayTime);

    switch (ones)
    {
    case 0:
        PORTB = 0b00111111;
        break; // 0
    case 1:
        PORTB = 0b00000110;
        break; // 1
    case 2:
        PORTB = 0b01011011;
        break; // 2
    case 3:
        PORTB = 0b01001111;
        break; // 3
    case 4:
        PORTB = 0b01100110;
        break; // 4
    case 5:
        PORTB = 0b01101101;
        break; // 5
    case 6:
        PORTB = 0b01111101;
        break; // 6
    case 7:
        PORTB = 0b00000111;
        break; // 7
    case 8:
        PORTB = 0b01111111;
        break; // 8
    case 9:
        PORTB = 0b01101111;
        break; // 9
    }
    _delay_ms(delayTime);
    PORTB = 0; // Turn off display
}