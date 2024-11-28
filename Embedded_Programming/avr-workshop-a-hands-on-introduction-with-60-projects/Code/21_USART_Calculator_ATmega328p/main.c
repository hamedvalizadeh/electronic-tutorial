#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <util/delay.h>

#define USART_BAUDRATE 4800
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

// send a string to USART
void sendString(char myString[10])
{
    uint8_t a = 0;
    for (a = 0; a < strlen(myString); a++)
    {
        USARTSendByte(myString[a]);
    }
}

// receives a byte of data from the computer into the USART register
uint8_t USARTReceiveByte()
{
    // Wait for byte from computer
    while (!(UCSR0A & (1 << RXC0)))
    {
    };
    // return byte
    return UDR0;
}

uint8_t get_digit()
{
    uint8_t num;
    num = USARTReceiveByte();
    USARTSendByte(num);
    // convert ASCII code of digit1 to actual number
    switch (num)
    {
    case 48:
        num = 0;
        break;
    case 49:
        num = 1;
        break;
    case 50:
        num = 2;
        break;
    case 51:
        num = 3;
        break;
    case 52:
        num = 4;
        break;
    case 53:
        num = 5;
        break;
    case 54:
        num = 6;
        break;
    case 55:
        num = 7;
        break;
    case 56:
        num = 8;
        break;
    case 57:
        num = 9;
        break;
    }
    return num;
}

int main(void)
{
    uint8_t digit1;
    uint8_t digit2;
    uint8_t operator;
    uint8_t answer = 0;

    char a[26] = "Enter command (e.g. 5*2) ";
    char b[11] = "Answer is ";
    char answerString[20] = ""; // holds answer
    char newline[4] = "\r\n";
    USARTInit();
    while (1)
    {
        sendString(newline);
        sendString(a);

        digit1 = get_digit();

        operator= USARTReceiveByte();
        USARTSendByte(operator);

        digit2 = get_digit();

        sendString(newline);

        // calculate result
        switch (operator)
        {
        case 43:
            answer = digit1 + digit2;
            break; // add
        case 45:
            answer = digit1 - digit2;
            break; // subtract
        case 42:
            answer = digit1 * digit2;
            break; // multiply
        case 47:
            answer = digit1 / digit2;
            break; // divide
        }

        // send result to PC via USART
        sendString(b);
        dtostrf(answer, 6, 2, answerString);
        sendString(answerString);
        sendString(newline);
        _delay_ms(1000);
    }
    return 0;
}
