// i2c.h
#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <stdbool.h>

void i2c_init();
bool i2c_wait();
uint8_t i2c_status();
bool i2c_start();
bool i2c_write(uint8_t data);
uint8_t i2c_read_nack();
uint8_t i2c_read_ack();
bool i2c_stop();
bool i2c_start_address(unsigned char address);

typedef struct {
    uint32_t scl_freq_hz;
    uint8_t prescaler;
    uint32_t wait_ms;
    uint8_t retry_count;
} i2c_config_t;

extern i2c_config_t I2C_CONFIG;

#endif