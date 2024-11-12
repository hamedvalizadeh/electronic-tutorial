// Project 12 – Building a Single-Digit Numerical Counter

#include <avr/io.h>
#include <util/delay.h>
#define TIME 150
 
void displayNumber(uint8_t value) 
// Displays numbers 0-9 on a seven-segment LED display
{
    switch(value)
    {
        case 0 : PORTB = 0b01000000; break; // 0
        case 1 : PORTB = 0b01111001; break; // 1 
        case 2 : PORTB = 0b00100100; break; // 2 
        case 3 : PORTB = 0b00110000; break; // 3 
        case 4 : PORTB = 0b00011001; break; // 4 
        case 5 : PORTB = 0b00010010; break; // 5 
        case 6 : PORTB = 0b00000010; break; // 6 
        case 7 : PORTB = 0b01111000; break; // 7 
        case 8 : PORTB = 0b00000000; break; // 8 
        case 9 : PORTB = 0b00010000; break; // 9 
    }
}

int main(void)
{
    uint8_t i=0;          // Counter value
    DDRB = 0b11111111;    // Set PORTB to outputs 
    DDRD = 0b00000000;    // Set PORTD to inputs
    PORTD = 0b11111100;   // Turn off internal pullups for PD0 and PD1
    for(;;)
    {
        displayNumber(i); // Display count 
        _delay_ms(TIME);
        if (PIND == 0b11111110) // If reset button pressed
        {
            i = 0;         // Set counter to zero
        }
        if (PIND == 0b11111101) // If count button pressed
        {
            i++;           // Increase counter
            if (i > 9)     // If counter is greater than 9
            {
                i = 0;      // Set counter to zero
            }
        }
    }
    return 0;             
}
