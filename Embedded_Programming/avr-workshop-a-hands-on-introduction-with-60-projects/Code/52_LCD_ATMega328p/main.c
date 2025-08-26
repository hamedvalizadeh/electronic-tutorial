#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>

void init_port()
{
    DDRD |= (1 << PORTD0);
    DDRD |= (1 << PORTD1);
    DDRD |= (1 << PORTD4);
    DDRD |= (1 << PORTD5);
    DDRD |= (1 << PORTD6);
    DDRD |= (1 << PORTD7);

    _delay_ms(100);
}

void lcd_latche_data()
{
    // Set E pin of the LCD to 1 and after 1 micro second delay again set it back to 0, to signal it to fetch the data
    PORTD |= (1 << PORTD1);
    _delay_us(1);
    PORTD &= ~(1 << PORTD1);
    _delay_us(200);
}

void lcd_send_data(uint8_t data, bool iscmd)
{
    // Set RS pin of the LCD to 1, to send data
    PORTD |= (1 << PORTD0);
    if (iscmd)
    {
        // Set RS pin of the LCD to 0, to send command
        PORTD &= ~(1 << PORTD0);
    }

    // Set high nibble of command byte in 4 MSB bits of PORTD register
    PORTD = (PORTD & 0b00001111) | (data & 0b11110000);
    lcd_latche_data();

    // Set low nibble of command byte in 4 MSB bits of PORTD register
    PORTD = (PORTD & 0b00001111) | (data << 4);
    lcd_latche_data();

    _delay_ms(2);
}

void lcd_write(uint8_t data)
{
    lcd_send_data(data, false);
}

void lcd_cmd(uint8_t cmd)
{
    lcd_send_data(cmd, true);
}

void lcd_init()
{
    // 0x02: Return Home (set cursor to 0,0). Also part of 4-bit init sequence.
    lcd_cmd(0x02);

    // 0x28: Function Set → 4-bit mode (DL=0), 2-line display (N=1), 5x8 font (F=0).
    lcd_cmd(0x28);

    // 0x0C: Display ON, Cursor OFF, Blink OFF. (D=1, C=0, B=0).
    lcd_cmd(0x0C);

    // 0x06: Entry Mode Set → increment cursor (I/D=1), no display shift (S=0).
    lcd_cmd(0x06);

    // 0x01: Clear Display → clear DDRAM and return cursor to home (0,0).
    lcd_cmd(0x01);

    // Delay needed because Clear Display (0x01) takes longer (~1.52ms).
    _delay_ms(2);
}

void lcd_print(char *data)
{
    uint8_t i;
    for (i = 0; data[i] != 0; i++)
    {
        lcd_write(data[i]);
    }
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    if (row == 0 && col < 16)
    {
        lcd_cmd((col & 0x0F) | 0x80); // Row 0, col 0–15
    }
    else if (row == 1 && col < 16)
    {
        lcd_cmd((col & 0x0F) | 0xC0); // Row 1, col 0–15
    }
}

void lcd_clear()
{
    lcd_cmd(0x01);
    _delay_ms(2);
    lcd_set_cursor(0, 0);
    _delay_ms(2);
}

int main(void)
{
    DDRB |= (1 << PORTB0);
    init_port();
    lcd_init();

    char numbers[9];
    uint8_t i;
    while (1)
    {
        lcd_set_cursor(1, 0);
        lcd_print("AVR Workshop");

        lcd_set_cursor(0, 1);
        lcd_print("Learning LCD Use");

        _delay_ms(1000);

        lcd_clear();

        lcd_set_cursor(1, 0);
        lcd_print("Counting Up");
        for (i = 0; i < 10; i++)
        {
            itoa(i, numbers, 10);
            lcd_set_cursor(i, 1);
            lcd_print(numbers);
            _delay_ms(1000);
        }
        lcd_clear();
    }

    return 0;
}