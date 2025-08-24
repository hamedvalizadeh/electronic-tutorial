// spi.h
#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

void spi_init(void);
void spi_transfer(uint8_t data);
void spi_wait(void);
void spi_start_transision(void);
void spi_stop_transision(void);

#endif
