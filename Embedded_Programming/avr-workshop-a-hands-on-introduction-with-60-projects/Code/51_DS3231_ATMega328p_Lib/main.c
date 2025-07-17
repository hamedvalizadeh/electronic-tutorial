#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "conversion.h"
#include "i2c.h"

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