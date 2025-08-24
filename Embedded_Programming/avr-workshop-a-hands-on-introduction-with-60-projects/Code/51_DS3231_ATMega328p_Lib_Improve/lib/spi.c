// spi.c
#include "spi.h"

void setup_port()
{
    // Set PORTB0 as output; to control the slave select action.
    DDRB |= (1 << PORTB0);
    // Set PORTB3 as output; to control data transfer from master to slave (MOSI).
    DDRB |= (1 << PORTB3);
    // Set PORTB5 as output; to control clock rate generated master.
    DDRB |= (1 << PORTB5);

    // Set the value of PORTB0 to HIGH, to disable data transfer between master microcontroller,
    // with slave connected to PINB0.
    PORTB |= (1 << PORTB0);
}

void spi_init(void)
{
    setup_port();

    // Enable SPI
    SPCR |= (1 << SPE);
    // Set this microcontroller as master
    SPCR |= (1 << MSTR);

    // DORD = 0: Config SPI to transfer and recieve MSB first
    // CPOL=0: SCK is low when idle
    // CPOL=0, CPHA=0 => Mode 0,
    //                   in whitch data is sampled on the rising edge of the clock
    //                   and shifted out on the falling edge, matching the MAX7219 timing requirements.

    // HINT: as we did not change the default value of following bits:
    //           - bit SPI2X from SPSR register
    //           - bit SPR0 from SPCR register
    //           - bit SPR1 from SPCR register
    //       so the division factor by which clock frequency of microcontroller is devided to set the speed of SPI clock rate, will be 4
}

void spi_wait(void)
{
    // Wait until data is transtered. when data transfer is completed,
    // the bit SPIF will be set. so we check the value of this
    // continusely, until its is 0 we do not let the micro to do anything.
    while (!(SPSR & (1 << SPIF)))
    {
    }
}

void spi_transfer(uint8_t data)
{
    SPDR = data;
    spi_wait();
}

void spi_start_transision(void)
{
    // Pull SS/Load low to start transmission[5][6]
    PORTB &= ~(1 << PB0);
}

void spi_stop_transision(void)
{
    // Pull SS/Load high to latch data[6][7]
    PORTB |= (1 << PB0);
}