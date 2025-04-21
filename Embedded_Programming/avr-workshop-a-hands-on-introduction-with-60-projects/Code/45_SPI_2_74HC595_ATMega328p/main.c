#include <avr/io.h>
#include <util/delay.h>

void setup_port()
{
    // Set PORTB0 as output; to control the slave select action.
    DDRB |= (1 << PORTB0);
    // Set PORTB3 as output; to control data transfer from master to slave (MOSI).
    DDRB |= (1 << PORTB3);
    // Set PORTB5 as output; to control clock rate generated master.
    DDRB |= (1 << PORTB5);

    // Set the value of PORTB0 to HIGH, to disable data transfer between master microcontroller, with slave connected to PINB0.
    PORTB |= (1 << PORTB0);
}

void setup_spi()
{
    // Enable SPI
    SPCR |= (1 << SPE);
    // Config SPI to transfer and recieve LSB first
    SPCR |= (1 << DORD);
    // Set this microcontroller as master
    SPCR |= (1 << MSTR);

    // HINT: as we did not change the default value of following bits:
    //           - bit SPI2X from SPSR register
    //           - bit SPR0 from SPCR register
    //           - bit SPR1 from SPCR register
    //       so the division factor by which clock frequency of microcontroller is devided to set the speed of SPI clock rate, will be 4
}

void wait_for_data_transfer()
{
    // Wait until data is transtered. when data transfer is completed, the bit SPIF will be set. so we check the value of this
    // continusely, until its is 0 we do not let the micro to do anything.
    while (!(SPSR & (1 << SPIF)))
    {
    }
}

void send_digit(uint8_t digit)
{
    uint8_t digit_data[] = {0b11111100, 0b01100000, 0b11011010,
                            0b11110010, 0b01100110, 0b10110110,
                            0b10111110, 0b11100000, 0b11111110,
                            0b11100110};
    SPDR = digit_data[digit];
    wait_for_data_transfer();
}

void transfer_digit_as_byte(uint8_t value)
{
    uint8_t left_digit;
    uint8_t right_digit;

    left_digit = value / 10;
    right_digit = value % 10;

    // Set the value of PORTB0 to LOW, to enable the connected slave to PINB0, to transfer data to it.
    PORTB &= ~(1 << PORTB0);

    send_digit(right_digit);
    send_digit(left_digit);

    // Set the value of PORTB0 to HIGH, to disable data transfer between master microcontroller, with slave connected to PINB0.
    PORTB |= (1 << PORTB0);
}

int main(void)
{
    uint8_t digit = 0;
    setup_port();
    setup_spi();

    while (1)
    {
        for (digit = 0; digit < 100; digit++)
        {
            transfer_digit_as_byte(digit);
            _delay_ms(250);
        }
    }
    return 0;
}