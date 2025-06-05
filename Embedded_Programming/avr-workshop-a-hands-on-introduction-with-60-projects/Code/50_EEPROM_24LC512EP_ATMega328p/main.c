#include <avr/io.h>
#include <util/delay.h>

#define EEPRON_24LC512_ADDR 0x50 // A2:A1:A0 = 0b000 → base address
#define EEPRON_24LC512_ADDR_WRITE (EEPRON_24LC512_ADDR << 1)
#define EEPRON_24LC512_ADDR_READ (EEPRON_24LC512_ADDR_WRITE + 1)

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

// ----------------------- SPI
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

void spi_data_transfer_wait()
{
    // Wait until data is transtered. when data transfer is completed,
    // the bit SPIF will be set. so we check the value of this
    // continusely, until its is 0 we do not let the micro to do anything.
    while (!(SPSR & (1 << SPIF)))
    {
    }
}
// ----------------------- SPI

// ----------------------- I2C
void i2c_stop()
{
    // stop I2C bus and release GPIO pins
    // clear interrupt, enable I2C, generate stop condition
    // send stop to terminate write operation
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    // Wait until stop condition is executed and I2C bus is released
    while (TWCR & (1 << TWSTO))
        ;
}

void i2c_wait()
{
    // wait until I2C finishes an operation
    // Presumably waits for TWINT to be set again → operation finished
    // Wait until the value of TWINT set to 1. means data transmission completed
    while (!(TWCR & (1 << TWINT)))
        ;
}

void i2c_write(uint8_t data)
// send 'data' to I2C bus
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    i2c_wait();
}

uint8_t i2c_read()
// Read incoming byte of data from I2C bus
{
    TWCR |= (1 << TWINT) | (1 << TWEN);
    i2c_wait();
    // Incoming byte is placed in TWDR register
    return TWDR;
}

uint8_t i2c_read_ack()
// Read incoming byte of data from I2C bus and ACK signal
{
    TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    i2c_wait();
    // Incoming byte is placed in TWDR register
    return TWDR;
}

uint8_t get_i2c_status()
{
    // Mask three LSB bits:
    // - Only keep bits 7–3 (status code)
    // - Discard prescaler/reserved bits (bits 2–0)
    return TWSR & 0b11111000;
}

void i2c_enable()
{
    // by default TWPS0 and TWPS1 are equal to 0, thus pre-scalar value is 1
    //  the formula of I2C SCL speed is (( Fcpu / ( 16 + (2 * TWBR * pre-scalar) )  )) =>
    //  SCL = 16000000 / (16 + (2 * 72 * 1)) => SCL = 100000 Hz
    TWBR = 72;
    // Enable I2C on PORTC4 (SDA) and PORTC5 (SCL)
    TWCR |= (1 << TWEN);
}

void i2c_start_init()
{
    uint8_t status;
    while (1)
    {
        // Send START condition
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        i2c_wait();

        status = get_i2c_status();

        // Checks if:
        // status = START condition transmitted [0x08 (0b00001000)]
        // or
        // status = Repeated START transmitted [0x10 (0b00010000)]
        if ((status == 0b00001000) || (status == 0b00010000))
        {
            // start condition acknowledgement received. so we can break the loop and continue the code
            break;
        }
        else
        {
            // If not → something went wrong → retry loop.
            continue;
        }
    }
}

void i2c_start_address(unsigned char address)
{
    uint8_t status;
    while (1)
    {
        i2c_start_init();
        i2c_write(address);

        status = get_i2c_status();

        // Checks if NACK received then send stop condition and retry the loop from the first line:
        // status = SLA+W sent, NACK received [0x20 (0b00100000)]
        // or
        // status = data byte received + NACK (used in receiver mode) [0x58 (0b01011000)]
        if ((status == 0b00100000) || (status == 0b01011000))
        {
            // Here we received NACK so we know that Secondary device is busy.
            i2c_stop();
            continue;
        }
        break;
    }
}
// ----------------------- I2C

// ---------------------------------- 24LC512
void eeprom_24lc512_start_and_set_memmory_address(uint16_t memmory_address)
{
    // start write mode communication with 24LC512 IC
    i2c_start_address(EEPRON_24LC512_ADDR_WRITE);
    // Set High byte of mmemmory address
    i2c_write(memmory_address >> 8);
    // Set Low byte of mmemmory address
    i2c_write(memmory_address);
}

uint16_t eeprom_24lc512_read(uint16_t memmory_address)
{
    eeprom_24lc512_start_and_set_memmory_address(memmory_address);
    // start read mode communication with 24LC512 IC
    i2c_start_address(EEPRON_24LC512_ADDR_READ);
    return i2c_read();
}

void init_mcp23017()
{
}
// ---------------------------------- 24LC512

// ----------------------------------- MAX7219
void max7219_write(uint8_t address, uint8_t data)
{
    // Pull SS/Load low to start transmission[5][6]
    PORTB &= ~(1 << PB0);

    // Send address byte
    SPDR = address;
    spi_data_transfer_wait();

    // Send data byte
    SPDR = data;
    spi_data_transfer_wait();

    // Pull SS/Load high to latch data[6][7]
    PORTB |= (1 << PB0);
}

void max7219_setup()
{
    // Disable test mode (often enabled by default)[8]
    // Test register: 0x0F, Disable: 0x00
    max7219_write(0x0F, 0x00);

    // Set decode mode (e.g., Code B for 7-segment)[7]
    // Decode all digits (0x09 = Decode Mode)
    max7219_write(0x09, 0xFF);

    // Set scan limit to 8 digits[7]
    // Scan limit: 0x0B, 8 digits: 0x07
    max7219_write(0x0B, 0x07);

    // Set brightness (0x0A: Intensity, 0x0F = max)[7]
    max7219_write(0x0A, 0x0F);

    // Enable normal operation[7]
    // Shutdown register: 0x0C, Enable: 0x01
    max7219_write(0x0C, 0x01);
}

void max7219_blank()
{
    uint8_t i = 0;
    for (i = 1; i < 9; i++)
    {
        // Send blank (15) to digit register (i)
        max7219_write(i, 15);
    }
}

void max7219_display_number(uint8_t digit, uint8_t number, uint8_t dp)
{
    if (dp == 1)
    {
        number = number + 128;
    }

    max7219_write(digit, number);
}

void max7219_send_number(uint32_t value)
{
    uint8_t digits[9];
    uint8_t number = 1;
    uint8_t dp = 0;

    for (number = 1; number < 9; number++)
    {
        // this value will clear digit on MAX7219
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
        max7219_display_number(number, digits[number], dp);
    }
}
// ----------------------------------- MAX7219

int main(void)
{
    setup_port();
    setup_spi();
    max7219_setup();
    max7219_blank();

    i2c_enable();

    int16_t i;
    uint16_t j;
    while (1)
    {
        max7219_send_number(11111111);
        _delay_ms(500);
        // Write loop
        for (i = 0; i < 256; i++)
        {
            eeprom_24lc512_start_and_set_memmory_address(i);
            // write actual data
            i2c_write(i);
            i2c_stop();
            max7219_send_number(i);
            _delay_ms(1);
        }

        max7219_send_number(11111111);
        _delay_ms(500);
        // Read loop
        for (i = 5; i >= 0; i--)
        {
            j = eeprom_24lc512_read(i);
            i2c_stop();
            max7219_send_number(j);
            _delay_ms(500);
        }

        _delay_ms(100);
    }

    return 0;
}