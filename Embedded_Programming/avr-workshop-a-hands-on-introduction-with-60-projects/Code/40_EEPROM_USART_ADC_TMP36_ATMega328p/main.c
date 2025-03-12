#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define USART_BAUDRATE 4800
#define UBRR_VALUE 12
#define LOG_DELAY 50

char newline[4] = "\r\n";

void port_setup()
{
    DDRC &= ~(1 << PORTC5);
}

//************* BEGIN: Usart *************

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
//************* END: Usart *************

//************* BEGIN: ADC *************

void setup_adc()
{
    // select AVCC (pin 20 of the ATMega328p) as the voltage source for ADC circuit.
    ADMUX |= (1 << REFS0);

    // select ADC5 (pin 28 of the ATMega328p)
    ADMUX |= (1 << MUX2) | (1 << MUX0);

    // set ADC pre-scalar as 8
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0);

    // enable ADC
    ADCSRA |= (1 << ADEN);
}

uint16_t read_value_adc()
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & ((1 << ADSC)))
        ;
    _delay_ms(10);
    return ADC;
}

float adc_to_temp_celcius()
{
    uint16_t adc_val = read_value_adc();
    float volt = (adc_val * 5);
    volt = volt / 1024;
    float temp = ((volt - 0.5) * 100);
    return temp;
}

//************* END: ADC *************

void log_data()
{
    float port_data = 0;
    uint16_t location = 0;

    char read_data[4] = "";

    char log_msg[] = "Logging Data ... ";
    send_string(log_msg);
    send_string(newline);

    for (location = 0; location < 1021; location = location + 4)
    {
        // Update location with port_data
        port_data = adc_to_temp_celcius();
        eeprom_update_float((float *)location, port_data);

        dtostrf(port_data, 4, 3, read_data);
        send_string(read_data);
        usart_send_byte('_');

        _delay_ms(LOG_DELAY);
    }
    send_string(newline);
}

void dump_data()
{
    float port_data = 0;
    uint16_t location = 0;

    char read_data[4] = "";
    char dump_msg[] = "Dumping Data ... ";
    send_string(dump_msg);
    send_string(newline);

    for (location = 0; location < 1021; location = location + 4)
    {
        port_data = eeprom_read_float((float *)location);
        dtostrf((float)port_data, 4, 3, read_data);
        send_string(read_data);
        usart_send_byte('_');
    }
    send_string(newline);
}

void erase_data()
{
    uint16_t location = 0;

    char log_msg[] = "Erasing Data ... ";
    send_string(log_msg);

    for (location = 0; location < 1024; location++)
    {
        eeprom_write_word((uint16_t *)location, 0);
        usart_send_byte('*');
    }

    char finish_msg[] = "Erasing Finished";
    send_string(finish_msg);
    send_string(newline);
}

int main(void)
{
    port_setup();
    setup_adc();
    usart_setup();

    char msg1[44] = "Enter 1 to start, 2 to dump, 3 to erase: ";
    uint8_t option;

    while (1)
    {
        send_string(msg1);
        option = usart_recieve_byte();
        usart_send_byte(option);
        send_string(newline);

        switch (option)
        {
        case 49:
            log_data();
            break;
        case 50:
            dump_data();
            break;
        case 51:
            erase_data();
            break;
        }
    }
    return 0;
}
