#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define SLAVE_ADDR 0x08
#define SLAVE_WRITE (SLAVE_ADDR << 1)

// ─── Prototypes ──────────────────────
void i2c_write(uint8_t data);
void i2c_stop(void);
void i2c_start_address(uint8_t address);
void i2c_enable(void);
void send_json_to_slave(const char *json);

// ───── I2C Core Functions ────────────────────────────────

void i2c_wait()
{
    while (!(TWCR & (1 << TWINT)))
        ;
}

void i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    i2c_wait();
}

void i2c_stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    while (TWCR & (1 << TWSTO))
        ;
}

uint8_t get_i2c_status()
{
    return TWSR & 0b11111000;
}

void i2c_start_init()
{
    uint8_t status;
    while (1)
    {
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        i2c_wait();

        status = get_i2c_status();
        if ((status == 0x08) || (status == 0x10)) // START or REPEATED START
            break;
    }
}

void i2c_start_address(uint8_t address)
{
    uint8_t status;
    while (1)
    {
        i2c_start_init();
        i2c_write(address);

        status = get_i2c_status();
        if ((status == 0x20) || (status == 0x58)) // SLA+W NACK or Data NACK
        {
            i2c_stop();
            continue;
        }
        break;
    }
}

void i2c_enable()
{
    TWBR = 72;           // 100 kHz I2C with F_CPU = 16 MHz
    TWCR |= (1 << TWEN); // Enable TWI hardware
}

// ───── Application Layer ────────────────────────────────

void send_json_to_slave(const char *json)
{
    uint16_t len = strlen(json); // Allow variable-length strings

    i2c_start_address(SLAVE_WRITE); // Begin communication

    // Send 16-bit length, MSB first
    i2c_write((len >> 8) & 0xFF); // High byte
    i2c_write(len & 0xFF);        // Low byte

    uint16_t i = 0;
    for (i = 0; i < len; i++)
    {
        i2c_write(json[i]); // Send each character
    }

    i2c_stop(); // End communication
}

int main(void)
{
    i2c_enable();

    while (1)
    {
        send_json_to_slave("{\"led\":1}");
        _delay_ms(1000);

        send_json_to_slave("{\"led\":0}");
        _delay_ms(1000);
    }

    return 0;
}
