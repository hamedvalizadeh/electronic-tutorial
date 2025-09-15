#include <stdlib.h>
#include <stdio.h>
#include "i2c.h"
#include "conversion.h"
#include "ds3232.h"

#define DS3232_ADDR 0x68
#define DS3232_ADDR_WRITE (DS3232_ADDR << 1)
#define DS3232_ADDR_READ (DS3232_ADDR_WRITE + 1)

void ds3232_bus_init(uint32_t scl_freq, uint8_t prescaler, uint8_t wait_ms, uint8_t retries)
{
    I2C_CONFIG.scl_freq_hz = scl_freq;
    I2C_CONFIG.prescaler = prescaler;
    I2C_CONFIG.wait_ms = wait_ms;
    I2C_CONFIG.retry_count = retries;
    i2c_init();
}

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

rtc_time_t ds3232_read_date_and_time()
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

void get_formated_date(rtc_time_t time, char *buffer, size_t buffer_size)
{
    snprintf(buffer, buffer_size, "%02d/%02d/%02d",
             time.year, time.month, time.date);
}

void get_formated_time(rtc_time_t time, char *buffer, size_t buffer_size)
{
    snprintf(buffer, buffer_size, "%02d:%02d:%02d",
             time.hours, time.minutes, time.seconds);
}