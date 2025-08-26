#include "lcd_lq.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>

lcd_config_t LCD_CONFIG = {};

// Common row address maps (HD44780 standard)
static const uint8_t ROW_OFFSETS_16x2[2] = {0x00, 0x40};
static const uint8_t ROW_OFFSETS_20x4[4] = {0x00, 0x40, 0x14, 0x54};
static const uint8_t ROW_OFFSETS_40x2[2] = {0x00, 0x40};

const uint8_t *SELECTED_ROW_OFFSETS;

void lcd_select_row_offsets()
{
    if (LCD_CONFIG.rows == 1)
    {
        // 1-line LCDs always start at 0x00
        static const uint8_t ROW_OFFSETS_1x[1] = {0x00};
        SELECTED_ROW_OFFSETS = ROW_OFFSETS_1x;
    }
    else if (LCD_CONFIG.rows == 2 && LCD_CONFIG.cols <= 16)
    {
        SELECTED_ROW_OFFSETS = ROW_OFFSETS_16x2;
    }
    else if (LCD_CONFIG.rows == 2 && LCD_CONFIG.cols > 16)
    {
        SELECTED_ROW_OFFSETS = ROW_OFFSETS_40x2;
    }
    else if (LCD_CONFIG.rows == 4 && LCD_CONFIG.cols == 20)
    {
        SELECTED_ROW_OFFSETS = ROW_OFFSETS_20x4;
    }
    else
    {
        // fallback safe default (16x2 style)
        SELECTED_ROW_OFFSETS = ROW_OFFSETS_16x2;
    }
}

void init_port()
{
    *LCD_CONFIG.ddr |= (1 << LCD_CONFIG.rs);
    *LCD_CONFIG.ddr |= (1 << LCD_CONFIG.en);
    *LCD_CONFIG.ddr |= (1 << LCD_CONFIG.d4);
    *LCD_CONFIG.ddr |= (1 << LCD_CONFIG.d5);
    *LCD_CONFIG.ddr |= (1 << LCD_CONFIG.d6);
    *LCD_CONFIG.ddr |= (1 << LCD_CONFIG.d7);

    _delay_ms(100);
}

void lcd_latche_data()
{
    // Set E pin of the LCD to 1 and after 1 micro second delay again set it back to 0, to signal it to fetch the data
    *LCD_CONFIG.port |= (1 << LCD_CONFIG.en);
    _delay_us(1);
    *LCD_CONFIG.port &= ~(1 << LCD_CONFIG.en);
    _delay_us(200);
}

void lcd_send_data(uint8_t data, bool iscmd)
{
    // Set RS pin of the LCD to 1, to send data
    *LCD_CONFIG.port |= (1 << LCD_CONFIG.rs);
    if (iscmd)
    {
        // Set RS pin of the LCD to 0, to send command
        *LCD_CONFIG.port &= ~(1 << LCD_CONFIG.rs);
    }

    // Set high nibble of command byte in 4 MSB bits of LCD_CONFIG.port register
    *LCD_CONFIG.port = (*LCD_CONFIG.port & 0b00001111) | (data & 0b11110000);
    lcd_latche_data();

    // Set low nibble of command byte in 4 MSB bits of LCD_CONFIG.port register
    *LCD_CONFIG.port = (*LCD_CONFIG.port & 0b00001111) | (data << 4);
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
    init_port();

    lcd_select_row_offsets();

    // 0x02: Return Home (set cursor to 0,0). Also part of 4-bit init sequence.
    lcd_cmd(0x02);

    // 0x28: Function Set → 4-bit mode (DL=0), 2-line display (N=1), 5x8 font (F=0).
    uint8_t function_set = 0x20 | 0x08; // 4-bit mode + 2-line by default
    if (LCD_CONFIG.rows == 1)
    {
        function_set &= ~0x08; // clear N bit → 1 line
    }
    lcd_cmd(function_set);

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
    if (row >= LCD_CONFIG.rows || col >= LCD_CONFIG.cols)
        return; // out of bounds, do nothing

    uint8_t address = col + SELECTED_ROW_OFFSETS[row];
    lcd_cmd(0x80 | address); // 0x80 = Set DDRAM address
}

void lcd_clear()
{
    lcd_cmd(0x01);
    _delay_ms(2);
    lcd_set_cursor(0, 0);
    _delay_ms(2);
}