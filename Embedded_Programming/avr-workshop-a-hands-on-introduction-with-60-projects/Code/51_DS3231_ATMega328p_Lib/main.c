#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

#define DS3232_ADDR 0x68
#define DS3232_ADDR_WRITE (DS3232_ADDR << 1)
#define DS3232_ADDR_READ (DS3232_ADDR_WRITE + 1)

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} rtc_time_t;

uint8_t dec_to_bcd(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}

uint8_t bcd_to_dec(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

// ----------------------------------- MAX7219
void max7219_write(uint8_t address, uint8_t data)
{
    spi_start_transision();

    // Send address byte
    spi_transfer(address);

    // Send data byte
    spi_transfer(data);
    
    spi_stop_transision();
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

// ----------------------- I2C
void i2c_init()
{
    // by default TWPS0 and TWPS1 are equal to 0, thus pre-scalar value is 1
    //  the formula of I2C SCL speed is (( Fcpu / ( 16 + (2 * TWBR * pre-scalar) )  )) =>
    //  SCL = 16000000 / (16 + (2 * 72 * 1)) => SCL = 100000 Hz
    TWBR = 72;
    // Enable I2C on PORTC4 (SDA) and PORTC5 (SCL)
    TWCR = (1 << TWEN);
}

void i2c_wait()
{
    // wait until I2C finishes an operation
    // Presumably waits for TWINT to be set again → operation finished
    // Wait until the value of TWINT set to 1. means data transmission completed
    while (!(TWCR & (1 << TWINT)))
        ;
}

uint8_t i2c_status()
{
    // Mask three LSB bits:
    // - Only keep bits 7–3 (status code)
    // - Discard prescaler/reserved bits (bits 2–0)
    return TWSR & 0b11111000;
}

void i2c_start()
{
    uint8_t status;
    while (1)
    {
        // Send START condition
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        i2c_wait();

        status = i2c_status();

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
//  ----------------------- I2C

// ---------------------------------- ds32323
void ds3232_set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day_of_week, uint8_t date, uint8_t month, uint8_t year)
{
    i2c_start_address(DS3232_ADDR_WRITE);
    i2c_write(0x00); // Start writing at register 0x00

    i2c_write(dec_to_bcd(sec));         // 0x00 Seconds
    i2c_write(dec_to_bcd(min));         // 0x01 Minutes
    i2c_write(dec_to_bcd(hour));        // 0x02 Hours
    i2c_write(dec_to_bcd(day_of_week)); // 0x03 Day (1–7)
    i2c_write(dec_to_bcd(date));        // 0x04 Date (1–31)
    i2c_write(dec_to_bcd(month));       // 0x05 Month (1–12)
    i2c_write(dec_to_bcd(year));        // 0x06 Year (00–99)

    i2c_stop();
}

rtc_time_t ds3232_read_date_and_time_fixed()
{
    rtc_time_t time;

    i2c_start_address(DS3232_ADDR_WRITE);
    i2c_write(0x00); // Set register pointer to 0x00

    i2c_start();
    i2c_write(DS3232_ADDR_READ);

    time.seconds = bcd_to_dec(i2c_read_ack());
    time.minutes = bcd_to_dec(i2c_read_ack());
    time.hours = bcd_to_dec(i2c_read_ack());
    time.day = bcd_to_dec(i2c_read_ack());
    time.date = bcd_to_dec(i2c_read_ack());
    time.month = bcd_to_dec(i2c_read_ack());
    time.year = bcd_to_dec(i2c_read_nack()); // Final read with NACK

    i2c_stop();

    return time;
}

// ---------------------------------- ds32323

int main(void)
{
    spi_init();
    max7219_setup();
    max7219_blank();

    i2c_init();
    // // Set time to: 2025-07-15 Tuesday, 14:36:45
    // ds3232_set_time(45, 36, 14, 2, 15, 7, 25);

    // _delay_ms(1000);

    rtc_time_t current;
    uint32_t display_value;

    while (1)
    {
        current = ds3232_read_date_and_time_fixed();

        // Construct 8-digit value as HHMMSSDD
        display_value = current.hours * 1000000UL +
                        current.minutes * 10000UL +
                        current.seconds * 100UL +
                        current.date;

        max7219_blank();
        max7219_send_number(display_value);
        _delay_ms(1000);
    }

    return 0;
}