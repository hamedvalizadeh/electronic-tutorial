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

    // Set the value of PORTB0 to HIGH, to disable data transfer between master microcontroller,
    // with slave connected to PINB0.
    PORTB |= (1 << PORTB0);
}

void setup_spi()
{
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

void wait_for_data_transfer()
{
    // Wait until data is transtered. when data transfer is completed,
    // the bit SPIF will be set. so we check the value of this
    // continusely, until its is 0 we do not let the micro to do anything.
    while (!(SPSR & (1 << SPIF)))
    {
    }
}

void spi_write_max7219(uint8_t address, uint8_t data)
{
    // Pull SS/Load low to start transmission[5][6]
    PORTB &= ~(1 << PB0);

    // Send address byte
    SPDR = address;
    wait_for_data_transfer();

    // Send data byte
    SPDR = data;
    wait_for_data_transfer();

    // Pull SS/Load high to latch data[6][7]
    PORTB |= (1 << PB0);
}

void setup_max7219()
{
    // Disable test mode (often enabled by default)[8]
    // Test register: 0x0F, Disable: 0x00
    spi_write_max7219(0x0F, 0x00);

    // Set decode mode (e.g., Code B for 7-segment)[7]
    // Decode all digits (0x09 = Decode Mode)
    spi_write_max7219(0x09, 0xFF);

    // Set scan limit to 8 digits[7]
    // Scan limit: 0x0B, 8 digits: 0x07
    spi_write_max7219(0x0B, 0x07);

    // Set brightness (0x0A: Intensity, 0x0F = max)[7]
    spi_write_max7219(0x0A, 0x0F);

    // Enable normal operation[7]
    // Shutdown register: 0x0C, Enable: 0x01
    spi_write_max7219(0x0C, 0x01);
}

void blank_max7219()
{
    uint8_t i = 0;
    for (i = 1; i < 9; i++)
    {
        // Send blank (15) to digit register (i)
        spi_write_max7219(i, 15);
    }
}

void display_number_max7219(uint8_t digit, uint8_t number, uint8_t dp)
{
    if (dp == 1)
    {
        number = number + 128;
    }

    spi_write_max7219(digit, number);
}

send_number(uint32_t value)
{
    uint8_t digits[9];
    uint8_t number = 1;
    uint8_t dp = 1;

    for (number = 1; number < 9; number++)
    {
        // this value will clear digit on MAZ7219
        digits[number] = 15;
        if (dp == 1)
        {
            digits[number] = digits[number] - 128;
        }
    }

    number = 1;

    while (value > 0)
    {
        digits[number] = value % 10;
        value = value / 10;
        number++;
    }

    for (number = 1; number < 9; number++)
    {
        display_number_max7219(number, digits[number], dp);
    }
}

int main(void)
{
    setup_port();
    setup_spi();
    setup_max7219();
    blank_max7219();

    uint32_t i;
    while (1)
    {
        for (i = 0; i < 100000000; i++)
        {
            send_number(i);
            _delay_ms(50);
        }
    }
    return 0;
}