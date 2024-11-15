#include <avr/io.h>

#define USART_BAUDRATE 4800
#define UBRR_VALUE 12

void USARTInit(void)
{
    // Set baud rate registers
    UBRR0H = (uint8_t)(UBRR_VALUE>>8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    // Set data type to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    // enable transmission and reception
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

void USARTSendByte(uint8_t u8Data)
{
    //wait while previous byte has been sent
    while(!(UCSR0A&(1<<UDRE0))){};
    // Transmit data
    UDR0 = u8Data;
}

void USARTSendString(char msg[])
{
    uint8_t a = 0;
    while(msg[a])
    {
        USARTSendByte(msg[a]);
        a++;
    }
}

int main (void)
{
    char message[15] = "Hello, World\r\n";
    //Initialize USART0
    USARTInit();
    while(1)
    {
        USARTSendString(message);
    }
}