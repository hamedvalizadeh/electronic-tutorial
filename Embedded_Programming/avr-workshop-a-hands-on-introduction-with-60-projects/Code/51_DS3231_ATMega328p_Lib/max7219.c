#include "spi.h"

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