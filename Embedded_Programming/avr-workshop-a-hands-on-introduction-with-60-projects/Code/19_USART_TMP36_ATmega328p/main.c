#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <util/delay.h>

#define USART_BAUDRATE 4800
#define UBRR_VALUE 12

void setup_adc()
{
    // Set External Voltage Source
    ADMUX |= (1 << REFS0);
    // Select ADC5 chanell (PC5 - pin number 28)
    ADMUX |= (1 << MUX2) | (1 << MUX0);

    // enable ADC
    ADCSRA |= (1 << ADEN);
    // Set pre-scalar to 8 (we use internal 1 MHZ crystal, so the speed of 125 KHZ for ADC would be suitable)
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
}

uint16_t read_adc()
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {
    };
    _delay_ms(5);
    return ADC;
}

float adc_to_temp_celcius(uint16_t adc_val)
{
    float volt = (adc_val * 5);
    volt = volt / 1024;
    float temp = ((volt - 0.5) * 100);
    return temp;
}

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

void USARTSendByte(unsigned char u8Data) // send a byte to USART
{
    // wait while previous byte is sent
    while (!(UCSR0A & (1 << UDRE0)))
    {
    };
    // Transmit data
    UDR0 = u8Data;
}

void sendString(char myString[])
{
    uint8_t a = 0;
    while (myString[a])
    {
        USARTSendByte(myString[a]);
        a++;
    }
}

int main(void)
{
    float temp;

    char temp_string[10] = "";
    char temp_label[14] = "Temperature: ";
    char unit_lable[14] = " degrees C ";
    char newline[4] = "\r\n";

    DDRC = 0b00000000;
    USARTInit();
    setup_adc();

    while (1)
    {
        temp = adc_to_temp_celcius(read_adc());

        sendString(temp_label);
        dtostrf(temp, 6, 2, temp_string);
        sendString(temp_string);
        sendString(unit_lable);
        sendString(newline);
        _delay_ms(1000);
    }

    return 0;
}