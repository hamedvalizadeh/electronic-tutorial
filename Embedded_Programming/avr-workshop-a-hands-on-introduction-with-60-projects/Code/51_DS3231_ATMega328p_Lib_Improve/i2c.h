// i2c.h
#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

void i2c_init();
void i2c_wait();
uint8_t i2c_status();
void i2c_start();
void i2c_write(uint8_t data);
uint8_t i2c_read_nack();
uint8_t i2c_read_ack();
void i2c_stop();
void i2c_start_address(unsigned char address);

typedef struct {
    uint32_t scl_freq_hz;
    uint8_t prescaler;
} i2c_config_t;

// Remove 'const' to allow modification
extern i2c_config_t I2C_CONFIG;

#endif
