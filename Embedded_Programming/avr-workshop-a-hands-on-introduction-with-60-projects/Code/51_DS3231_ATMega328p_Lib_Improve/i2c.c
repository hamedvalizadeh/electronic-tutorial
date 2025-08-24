#include "i2c.h"

i2c_config_t I2C_CONFIG = {
    .scl_freq_hz = 100000,
    .prescaler = 1,
    .wait_ms = 10,
    .retry_count = 5};

typedef enum
{
    // ---- Master Transmitter Mode ----
    I2C_START_TRANSMITTED = 0x08, // 0b00001000: START condition transmitted
    I2C_REP_START = 0x10,         // 0b00010000: Repeated START condition transmitted
    I2C_SLA_W_ACK = 0x18,         // 0b00011000: SLA+W transmitted, ACK received
    I2C_SLA_W_NACK = 0x20,        // 0b00100000: SLA+W transmitted, NACK received
    I2C_DATA_ACK = 0x28,          // 0b00101000: Data transmitted, ACK received
    I2C_DATA_NACK = 0x30,         // 0b00110000: Data transmitted, NACK received
    I2C_ARB_LOST = 0x38,          // 0b00111000: Arbitration lost in SLA+W or data

    // ---- Master Receiver Mode ----
    I2C_SLA_R_ACK = 0x40,          // 0b01000000: SLA+R transmitted, ACK received
    I2C_SLA_R_NACK = 0x48,         // 0b01001000: SLA+R transmitted, NACK received
    I2C_DATA_RECEIVED_ACK = 0x50,  // 0b01010000: Data received, ACK returned
    I2C_DATA_RECEIVED_NACK = 0x58, // 0b01011000: Data received, NACK returned

    // ---- Slave Transmitter Mode ----
    I2C_SLA_R_RECEIVED_ACK = 0xA8, // 0b10101000: Own SLA+R received, ACK returned
    I2C_ARB_LOST_SLA_R_ACK = 0xB0, // 0b10110000: Arbitration lost, SLA+R received, ACK returned
    I2C_DATA_SENT_ACK = 0xB8,      // 0b10111000: Data transmitted, ACK received
    I2C_DATA_SENT_NACK = 0xC0,     // 0b11000000: Data transmitted, NACK received
    I2C_LAST_DATA_SENT_ACK = 0xC8, // 0b11001000: Last data transmitted, ACK received

    // ---- Slave Receiver Mode ----
    I2C_SLA_W_RECEIVED_ACK = 0x60,     // 0b01100000: Own SLA+W received, ACK returned
    I2C_ARB_LOST_SLA_W_ACK = 0x68,     // 0b01101000: Arbitration lost, SLA+W received, ACK returned
    I2C_GCALL_RECEIVED_ACK = 0x70,     // 0b01110000: General call received, ACK returned
    I2C_ARB_LOST_GCALL_ACK = 0x78,     // 0b01111000: Arbitration lost, general call received, ACK returned
    I2C_DATA_RECEIVED_SLA_ACK = 0x80,  // 0b10000000: Data received (after SLA+W), ACK returned
    I2C_DATA_RECEIVED_SLA_NACK = 0x88, // 0b10001000: Data received (after SLA+W), NACK returned
    I2C_DATA_RECEIVED_GC_ACK = 0x90,   // 0b10010000: Data received (after general call), ACK returned
    I2C_DATA_RECEIVED_GC_NACK = 0x98,  // 0b10011000: Data received (after general call), NACK returned
    I2C_STOP_REPEATED_START = 0xA0,    // 0b10100000: STOP or repeated START received while addressed as slave

    // ---- Miscellaneous ----
    I2C_NO_INFO = 0xF8,  // 0b11111000: No relevant state information (TWINT=0)
    I2C_BUS_ERROR = 0x00 // 0b00000000: Illegal START or STOP condition
} i2c_status_code_t;

static uint32_t i2c_ms_to_loops(uint32_t ms)
{
    // Assume F_CPU in Hz and a simple loop overhead.
    // Each iteration of while (!(TWCR & (1 << TWINT))) is ~4 CPU cycles
    // You may adjust this empirically
    // F_CPU / 1000 gives cycles per millisecond
    return (F_CPU / 1000UL / 4UL) * ms;
}

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
    // 0xFC = 0b11111100 => (TWSR & 0xFC) will set 2 LSB bits of the TWSR to 0
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
    uint32_t counter = i2c_ms_to_loops(I2C_CONFIG.wait_ms);

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
    uint8_t retries = I2C_CONFIG.retry_count;

    while (retries--)
    {
        // Send START condition
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

        if (!i2c_wait())
        {
            return false; // timeout on wait
        }

        status = i2c_status();

        if ((status == I2C_START_TRANSMITTED) || (status == I2C_REP_START))
        {
            return true; // success
        }
    }

    return false;
}

// Send 'data' to I2C bus, return true if successful
bool i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    if (!i2c_wait())
    {
        // timeout waiting for transmission
        return false;
    }

    uint8_t status = i2c_status();
    if (status == I2C_DATA_ACK)
    {
        // success: data transmitted, ACK received
        return true;
    }
    else if (status == I2C_DATA_NACK)
    {
        // Slave replied with NACK → device busy or not ready
        return false;
    }
    else
    {
        // Unexpected state
        return false;
    }
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

bool i2c_stop()
{
    uint32_t counter = i2c_ms_to_loops(I2C_CONFIG.wait_ms);

    // stop I2C bus and release GPIO pins
    // clear interrupt, enable I2C, generate stop condition
    // send stop to terminate write operation
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

    // Wait until stop condition is executed and I2C bus is released
    while (TWCR & (1 << TWSTO))
    {
        if (--counter == 0)
        {
            return false;
        }
    }

    return true;
}

bool i2c_start_address(unsigned char address)
{
    uint8_t status;
    uint8_t retries = I2C_CONFIG.retry_count;

    while (retries--)
    {
        if (!i2c_start())
        {
            return false;
        }

        if (!i2c_write(address))
        {
            return false;
        }

        status = i2c_status();

        // Checks if NACK received then send stop condition and retry the loop from the first line:
        if ((status == I2C_SLA_W_NACK) || (status == I2C_DATA_RECEIVED_NACK))
        {
            // Here we received NACK so we know that Secondary device is busy.
            i2c_stop();
            continue;
        }

        return true;
    }

    // Unexpected status → stop and fail
    i2c_stop();
    return false;
}