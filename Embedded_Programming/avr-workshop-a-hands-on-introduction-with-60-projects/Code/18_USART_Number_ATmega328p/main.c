#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define USART_BAUDRATE 4800
#define UBRR_VALUE 12

void USARTInit(void)
{
    // Set baud rate registers
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    // Set data type to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    // enable transmission and reception
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

void USARTSendByte(uint8_t u8Data)
{
    // wait while previous byte has been sent
    while (!(UCSR0A & (1 << UDRE0)))
    {
    };
    // Transmit data
    UDR0 = u8Data;
}

void USARTSendString(char msg[])
{
    uint8_t a = 0;
    while (msg[a])
    {
        USARTSendByte(msg[a]);
        a++;
    }
}

int main(void)
{
    char float_plc_h[10] = "Float - ";
    char int_plc_h[10] = "Integer - ";
    char result[10] = "";
    char new_line[4] = "\r\n";
    int16_t i = -32767;
    float f = -12345.67;

    // Initialize USART0
    USARTInit();
    while (1)
    {
        dtostrf((float)i, 12, 0, result);
        USARTSendString(int_plc_h);
        printf(result);
        USARTSendString(new_line);
        _delay_ms(1000);

        dtostrf(f, 12, 2, result);
        USARTSendString(float_plc_h);
        USARTSendString(result);
        USARTSendString(new_line);
        _delay_ms(1000);
    }
}