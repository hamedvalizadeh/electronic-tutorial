#include <avr/io.h>

#define USART_BAUDRATE 4800
#define UBRR_VALUE 12

void usart_setup(void)
{
    // Set baud rate registers
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    // Set data type to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    // enable transmission and reception
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

void usart_send_byte(uint8_t u8Data)
{
    // wait while previous byte has been sent
    while (!(UCSR0A & (1 << UDRE0)))
    {
    };
    // Transmit data
    UDR0 = u8Data;
}

uint8_t usart_recieve_byte()
{
    // Wait for byte from computer
    while (!(UCSR0A & (1 << RXC0)))
    {
    };
    // Return byte
    return UDR0;
}

void send_string(char msg[])
{
    uint8_t i = 0;
    while (msg[i])
    {
        usart_send_byte(msg[i]);
        i++;
    }
}