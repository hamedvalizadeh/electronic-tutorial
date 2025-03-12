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
    DDRB &= ~(1 << PORTB0);
    DDRB |= (1 << PORTB1);
}

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

void log_data()
{
    uint8_t port_data = 0;
    uint16_t location = 0;

    char read_data[1] = "";

    char log_msg[] = "Logging Data ... ";
    send_string(log_msg);
    send_string(newline);

    for (location = 0; location < 1024; location++)
    {
        // button on PINB0 is pressed.
        if ((PINB & (1 << PORTB0)))
        {
            port_data = 1;
            // LED ON
            PORTB |= (1 << PORTB1);
        }
        else
        {
            port_data = 0;
            // LED OFF
            PORTB &= ~(1 << PORTB1);
        }

        // Update location with port_data
        eeprom_update_word((uint16_t *)location, port_data);

        dtostrf((float)port_data, 1, 0, read_data);
        send_string(read_data);
        usart_send_byte('.');

        // Skip a location because a word need 2 bytes.
        location++;
        _delay_ms(LOG_DELAY);
    }
    send_string(newline);
}

void dump_data()
{
    uint8_t port_data = 0;
    uint16_t location = 0;

    char read_data[1] = "";
    char dump_msg[] = "Dumping Data ... ";
    send_string(dump_msg);
    send_string(newline);

    for (location = 0; location < 1024; location++)
    {
        port_data = eeprom_read_word((uint16_t *)location);
        dtostrf((float)port_data, 1, 0, read_data);
        send_string(read_data);
        usart_send_byte('.');

        // Skip a location because a word need 2 bytes.
        location++;
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
