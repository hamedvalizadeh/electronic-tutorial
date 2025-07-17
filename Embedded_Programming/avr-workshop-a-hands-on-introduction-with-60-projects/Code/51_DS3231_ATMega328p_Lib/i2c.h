// spi.h
#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

#define DS3232_ADDR 0x68
#define DS3232_ADDR_WRITE (DS3232_ADDR << 1)
#define DS3232_ADDR_READ (DS3232_ADDR_WRITE + 1)

void i2c_init();
void i2c_wait();
uint8_t i2c_status();
void i2c_start();
void i2c_write(uint8_t data);
uint8_t i2c_read_nack();
uint8_t i2c_read_ack();
void i2c_stop();
void i2c_start_address(unsigned char address);

#endif
