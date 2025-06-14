#include <avr/io.h>
#include <util/delay.h>

#define MCP23017_ADDR 0x20 // A2:A1:A0 = 0b000 → base address
#define MCP23017_WRITE (MCP23017_ADDR << 1)
#define MCP23017_IODIRA 0x00 // I/O direction register for Port A
#define MCP23017_IODIRB 0x01 // I/O direction register for Port B
#define MCP23017_GPIOA 0x12  // GPIO output register for Port A
#define MCP23017_GPIOB 0x13  // GPIO output register for Port B

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

void i2c_wait()
{
    // wait until I2C finishes an operation
    // Presumably waits for TWINT to be set again → operation finished
    // Wait until the value of TWINT set to 1. means data transmission completed
    while (!(TWCR & (1 << TWINT)))
        ;
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

void i2c_write(uint8_t data)
// send 'data' to I2C bus
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    i2c_wait();
}

void mcp23017_write_register(uint8_t reg, uint8_t data)
{
    i2c_start_address(MCP23017_WRITE); // Send address + write
    i2c_write(reg);                    // Register address
    i2c_write(data);                   // Register data
    i2c_stop();                        // End communication
}


void init_mcp23017()
{
    // All PORTA as output
    mcp23017_write_register(MCP23017_IODIRA, 0x00);
    // All PORTB as output
    mcp23017_write_register(MCP23017_IODIRB, 0x00);

    // mcp23017_write_register(0x01, 0xFF); // All PORTB as input
}

int main(void)
{
    uint8_t i;

    i2c_enable();
    init_mcp23017();

    while (1)
    {
        for (i = 0; i < 256; i++)
        {
            mcp23017_write_register(MCP23017_GPIOA, i);
            mcp23017_write_register(MCP23017_GPIOB, i);

            _delay_ms(100);
        }
    }
}