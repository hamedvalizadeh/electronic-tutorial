#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "tmp36.h"
#include "usart.h"
#include "adc.h"

char newline[4] = "\r\n";

void setup_port()
{
    setup_port_tmp();
    setup_port_adc();
}

int main(void)
{
    float tmp = 0;
    setup_port();
    setup_adc5_avcc();
    usart_setup();

    char read_tmp[4] = "";

    while (1)
    {
        tmp = read_tmp36_in_celcius(read_value_adc5());

        dtostrf(tmp, 4, 3, read_tmp);
        send_string(read_tmp);
        send_string(newline);

        print_digit((uint8_t)tmp);

        _delay_ms(1000);
    }
    return 0;
}