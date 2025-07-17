#include "i2c.h"
#include "ds32323.h"

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
