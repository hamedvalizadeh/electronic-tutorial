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

void transfer_digit_as_byte(uint8_t digit)
{
    // Set the value of PORTB0 to LOW, to enable the connected slave to PINB0, to transfer data to it.
    PORTB &= ~(1 << PORTB0);
    switch (digit)
    {
    case 0:
        SPDR = 0b11111100;
        break; // 0
    case 1:
        SPDR = 0b01100000;
        break; // 1
    case 2:
        SPDR = 0b11011010;
        break; // 2
    case 3:
        SPDR = 0b11110010;
        break; // 3
    case 4:
        SPDR = 0b01100110;
        break; // 4
    case 5:
        SPDR = 0b10110110;
        break; // 5
    case 6:
        SPDR = 0b10111110;
        break; // 6
    case 7:
        SPDR = 0b11100000;
        break; // 7
    case 8:
        SPDR = 0b11111110;
        break; // 8
    case 9:
        SPDR = 0b11100110;
        break; // 9
    }

    // Wait until data is transtered. when data transfer is completed, the bit SPIF will be set. so we check the value of this
    // continusely, until its is 0 we do not let the micro to do anything.
    while (!(SPSR & (1 << SPIF)))
    {
    }

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
        for (digit = 0; digit < 10; digit++)
        {
            transfer_digit_as_byte(digit);
            _delay_ms(1000);
        }
    }
    return 0;
}