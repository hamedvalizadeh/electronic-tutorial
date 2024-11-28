#include <avr/io.h>
#include <stdlib.h>

//BAUDRATE 4800
#define UBRR_VALUE 12

void USARTInit(void)
{
    // Set baud rate registers
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;

    // Set data frame format to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

    // enable transmission and reception
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

void USARTSendByte(uint8_t sentByte)
{
    // wait while previous byte is sent
    while (!(UCSR0A & (1 << UDRE0)))
    {
    };
    // Transmit data
    UDR0 = sentByte;
}

uint8_t USARTReceiveByte()
// receives a byte of data from the computer into the USART register
{
    // Wait for byte from computer
    while (!(UCSR0A & (1 << RXC0)))
    {
    };
    // return byte
    return UDR0;
}

int main(void)
{
    uint8_t tempByte;
    // Initialize USART0
    USARTInit();

    while (1)
    {
        // Receive data from PC via USART
        tempByte = USARTReceiveByte();

        // Send same data back to PC via USART
        USARTSendByte(tempByte);
    }
}