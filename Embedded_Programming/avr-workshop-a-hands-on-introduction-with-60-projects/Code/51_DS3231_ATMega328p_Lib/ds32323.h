#ifndef DS3232_H
#define DS3232_H

#include <avr/io.h>

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


void ds3232_set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day_of_week, uint8_t date, uint8_t month, uint8_t year);
rtc_time_t ds3232_read_date_and_time();
#endif
