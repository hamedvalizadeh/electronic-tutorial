#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "max7219.h"
#include "ds3232.h"

int main(void)
{
    spi_init();
    max7219_setup();
    max7219_blank();

    ds3232_bus_init(100000, 1, 15, 5);

    // HINT: uncomment following code to set time to ((2025-07-15 Tuesday, 14:36:45))
    // ds3232_set_time(45, 36, 14, 2, 15, 7, 25);
    // _delay_ms(1000);
    rtc_time_t current;
    uint32_t display_value;

    while (1)
    {
        current = ds3232_read_date_and_time();

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