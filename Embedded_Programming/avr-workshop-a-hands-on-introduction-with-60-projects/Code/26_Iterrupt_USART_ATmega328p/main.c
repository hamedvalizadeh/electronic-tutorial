#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define UBRR_VALUE 12

volatile uint8_t count_i = 100;
void setup_usart()
{
    // set prescalar baud rate to 12
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)(UBRR_VALUE);

    // set data frame size to 8 bits
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

    // enable receive and transmit
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

void usart_send_byte(uint8_t d)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
    };
    UDR0 = d;
}

void usart_send_string(char data[])
{
    uint8_t i = 0;
    while (data[i])
    {
        usart_send_byte(data[i]);
        i++;
    };
}

void send_count_message(char interrupt_name[])
{
    char str_i[10];
    // convert uint8_t to char array
    itoa(count_i, str_i, 10);

    usart_send_string(interrupt_name);
    usart_send_string(": ");
    usart_send_string(str_i);
    usart_send_string("\r\n");
}

// Interrupt function of ITN0
ISR(INT0_vect)
{
    count_i += 1;
    send_count_message("INT0");
    EIFR &= ~(1 << 0);
}

// Interrupt function of ITN1
ISR(INT1_vect)
{
    count_i += 1;
    send_count_message("INT1");
    EIFR &= ~(1 << 1);
}

void setup_interrupt()
{
    // turn on interrupt on pin 4 (INT0, PD2), and pin 5 (INT1, PD3)
    EIMSK |= (1 << INT0) | (1 << INT1);

    // set the trigger on rising edge for INT0
    EICRA |= (1 << ISC01) | (1 << ISC00);

    // set the trigger on rising edge for INT1
    EICRA |= (1 << ISC11) | (1 << ISC10);

    // enable global interrupt flag
    sei();

    // set the PD2 as input;
    DDRD &= ~(1 << PORTD2);
    // set the PD3 as input;
    DDRD &= ~(1 << PORTD3);
}

int main(void)
{
    setup_usart();
    setup_interrupt();

    while (1)
    {
    };
    return 0;
}