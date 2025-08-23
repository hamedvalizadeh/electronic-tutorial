#include "i2c.h"

#define I2C_TIMEOUT 50000UL // adjust depending on CPU speed

i2c_config_t I2C_CONFIG = {
    .scl_freq_hz = 100000, // default 100 kHz
    .prescaler = 1};

void i2c_init()
{
    uint32_t scl_freq_hz = I2C_CONFIG.scl_freq_hz;
    uint8_t prescaler = I2C_CONFIG.prescaler;

    uint8_t twps_val = 0;

    // Map prescaler value to TWSR bits
    switch (prescaler)
    {
    case 1:
        twps_val = 0;
        break; // 00
    case 4:
        twps_val = 1;
        break; // 01
    case 16:
        twps_val = 2;
        break; // 10
    case 64:
        twps_val = 3;
        break; // 11
    default:
        twps_val = 0;
        prescaler = 1;
        break; // fallback
    }

    // Set prescaler bits
    TWSR = (TWSR & 0xFC) | twps_val;

    // Compute TWBR value
    uint32_t twbr_val = ((F_CPU / scl_freq_hz) - 16) / (2UL * prescaler);

    if (twbr_val > 255)
    {
        twbr_val = 255; // clamp to 8-bit max
    }

    // by default TWPS0 and TWPS1 are equal to 0, thus pre-scalar value is 1
    //  the formula of I2C SCL speed is (( Fcpu / ( 16 + (2 * TWBR * pre-scalar) )  )) =>
    //  SCL = 16000000 / (16 + (2 * 72 * 1)) => SCL = 100000 Hz
    TWBR = (uint8_t)twbr_val;

    // Enable I2C on PORTC4 (SDA) and PORTC5 (SCL)
    TWCR = (1 << TWEN);
}

bool i2c_wait()
{
    uint32_t counter = I2C_TIMEOUT;

    // wait until I2C finishes an operation
    // Presumably waits for TWINT to be set again → operation finished
    // Wait until the value of TWINT set to 1. means data transmission completed
    while (!(TWCR & (1 << TWINT)))
    {
        if (--counter == 0)
        {
            return false;
        }
    }
    return true;
}

uint8_t i2c_status()
{
    // Mask three LSB bits:
    // - Only keep bits 7–3 (status code)
    // - Discard prescaler/reserved bits (bits 2–0)
    return TWSR & 0b11111000;
}

bool i2c_start()
{
    uint8_t status;
    uint16_t retries = 5; // try a few times before giving up

    while (retries--)
    {
        // Send START condition
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

        if (!i2c_wait())
        {
            return false; // timeout on wait
        }

        status = i2c_status();

        // Checks if:
        // status = START condition transmitted [0x08 (0b00001000)]
        // or
        // status = Repeated START transmitted [0x10 (0b00010000)]
        if ((status == 0b00001000) || (status == 0b00010000))
        {
            return true; // success
        }
    }

    return false; // failed after retries
}

// send 'data' to I2C bus
void i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    i2c_wait();
}

// Read incoming byte of data from I2C bus
uint8_t i2c_read_nack()
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    i2c_wait();
    // Incoming byte is placed in TWDR register
    return TWDR;
}

// Read incoming byte of data from I2C bus and ACK signal
uint8_t i2c_read_ack()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    i2c_wait();
    // Incoming byte is placed in TWDR register
    return TWDR;
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

// arrararara
void i2c_start_address(unsigned char address)
{
    uint8_t status;
    while (1)
    {
        i2c_start();
        i2c_write(address);

        status = i2c_status();

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