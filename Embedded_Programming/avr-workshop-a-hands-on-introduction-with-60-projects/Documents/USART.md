# Sample Basic Code

a sample basic code to implement `USART` in an`ATmega328p` with internal oscilloscope of 1 MHZ frequency enabled, with Baud Rate configured to be 4800, would be as follow:

```c
#include <avr/io.h>

#define USART_BAUDRATE 4800
#define UBRR_VALUE 12

void USARTInit(void)
{
    // Set baud rate registers
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    // Set data type to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    // enable transmission and reception
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

void USARTSendByte(uint8_t u8Data)
{
    // wait while previous byte has been sent
    while (!(UCSR0A & (1 << UDRE0)))
    {
    };
    // Transmit data
    UDR0 = u8Data;
}

void USARTSendString(char msg[])
{
    uint8_t a = 0;
    while (msg[a])
    {
        USARTSendByte(msg[a]);
        a++;
    }
}

int main(void)
{
    char message[15] = "Hello, World\r\n";
    // Initialize USART0
    USARTInit();
    while (1)
    {
        USARTSendString(message);
    }
}
```

  



# USART Hardware

in order to be able to connect an AVR micro controller to a computer, we need one of the following devices:

- RS232 protocol cable.
  - although protocol has 9 pins, but we just need 3 of them; RX, TX, and GND pins. 
- USB to Serial connector. there are many types of this devices that some of them would be based on the following ICs:
  - CP2102
  - FT232RL
  - CH340



# Registers

there are 6 registers in `AVR` microcontrollers, that by the help of them we can setup and configure `USART`.

- `UDR`

  - this is data register, that we can read data and write data to it. basically it consists of 2 resisters including `Tx` and `Rx`.

- `UCSRA`

  - USART Control and Status Register A. As the name suggests, it is used to control some needed behavior and then to monitor the result by the help of status flags.

- `UCSRB`

  - USART Control and Status Register A. As the name suggests, it is used to control some needed behavior and then to monitor the result by the help of status flags.

- `UCSRC`

  - USART Control and Status Register A. As the name suggests, it is used to control some needed behavior and then to monitor the result by the help of status flags.

- `UBRRL`

  - baud rate register. it is used to set the baud rate.

- `UBRRH`

  - baud rate register. it is used to set the baud rate.




# `UDR` Bits

as mentioned earlier, there are 2 `UDR` registers, one for receive and the other for transmit. these resisters share ethe same I/O address. 

if transmitter is enabled and data is written to the transmit, then the data will be loaded to the shift register.

if receiver is enabled, it is possible to read data from shift register. 



| Bit Name of TX | Bit Name of RX | Read/Write | Default Value |
| :------------: | :------------: | :--------: | :-----------: |
|      TXB7      |      RXB7      |    R/W     |       0       |
|      TXB6      |      RXB6      |    R/W     |       0       |
|      TXB5      |      RXB5      |    R/W     |       0       |
|      TXB4      |      RXB4      |    R/W     |       0       |
|      TXB3      |      RXB3      |    R/W     |       0       |
|      TXB2      |      RXB2      |    R/W     |       0       |
|      TXB1      |      RXB1      |    R/W     |       0       |
|      TXB0      |      RXB0      |    R/W     |       0       |



### `UCSRA` Bits

| Bit Name | Default Value | Read/Write | Description                                                  |
| :------: | :-----------: | :--------: | ------------------------------------------------------------ |
|   RXC    |       0       |     R      | receive complete. 1 is yes and means there is unread data on `UDR`. it could be used to generate a receive complete interrupt. (the interrupt should be enabled by `RXCIE` bit in `UDCSRB` register) |
|   TXC    |       0       |    R/W     | transfer is complete.  1 is yes and means there is no more data in the transmit register of `UDR`to be send. it could be used to generate a transmit complete interrupt. (the interrupt should be enabled by `TXCIE` bit in `UDCSRB` register) |
|   UDRE   |       1       |     R      | is data register empty? 1 is yes and indicates that the `UDR` register is ready to receive new data.  it could be set to generate a data register empty interrupt. (the interrupt should be enabled by `UDRIE` bit in `UDCSRB` register) |
|    FE    |       0       |     R      | frame error.                                                 |
|   DOR    |       0       |     R      | data overrun. if 1, it shows that buffer is full and a new character is waiting in the shift register. |
|    PE    |       0       |     R      | parity error.                                                |
|   U2X    |       0       |     R      | to double the transmission speed. if set, the speed would be doubled. |
|   MPCM   |       0       |     R      | multi-process communication mode. if set to 1 all the incoming frames, received by the receiver register will be ignored in do not contain address information. this bit has no effect on transmit register behavior. |



### `UCSRB` Bits

| Bit Name | Default Value | Read/Write | Description                                                  |
| :------: | :-----------: | :--------: | ------------------------------------------------------------ |
|  RXCIE   |       0       |    R/W     | RX Complete Interrupt Enable. 1 enable interrupt on RXC.     |
|  TXCIE   |       0       |    R/W     | TX Complete Interrupt Enable. 1 enable interrupt on TXC.     |
|  UDRIE   |       0       |    R/W     | data register empty Interrupt Enable. 1 enable interrupt on UDRE. |
|   RCEN   |       0       |    R/W     | Receiver Enable. 1 means enabled.                            |
|   TXEN   |       0       |    R/W     | Transmitter Enable. 1 means enabled.                         |
|  UCSZ2   |       0       |    R/W     | by combining this bit with `UCSZ1` and `UCSZ0` bits in `UCSRC` register, we can set the number of data bits (character size ) in a frame the receiver and transmitter use. |
|   RXB8   |       0       |    R/W     | Receive Data Bit 8                                           |
|   TXB8   |       0       |    R/W     | Transmit Data Bit 8                                          |



### `UCSRC` Bits

| Bit Name | Default Value | Read/Write | Description                                                  |
| :------: | :-----------: | :--------: | ------------------------------------------------------------ |
|  UMSEL1  |       0       |    R/W     | combined with `UMSEL0` to select operation mode of USART     |
|  UMSEL0  |       0       |    R/W     | combined with `UMSEL1` to select operation mode of USART     |
|   UPM1   |       0       |    R/W     | combined with `UPM0` to select parity mode of USART          |
|   UPM0   |       0       |    R/W     | combined with `UPM1` to select parity mode of USART          |
|   USBS   |       0       |    R/W     | setup the number of stop bits to be inserted in transmitter. 0 will result in 1 stop bit configuration, while 1 will result in 2 stop bits configuration. the receiver ignores this setting. |
|  UCSZ1   |       1       |    R/W     | by combining this bit with `UCSZ0` bit in `UCSRC` register and `UCSZ2` in UCSRB register, we can set the number of data bits (character size ) in a frame the receiver and transmitter use. |
|  UCSZ0   |       1       |    R/W     | by combining this bit with `UCSZ1 bit in `UCSRC` register and `UCSZ2` in UCSRB register, we can set the number of data bits (character size ) in a frame the receiver and transmitter use. |
|  UCPOL   |       0       |    R/W     | this bit is used in synchronous (set it to 0 in asynchronous mode). this bit set the relation between data output change and data input sample, and synchronous clock (XCK). when set to 0, output data changes on rising XCK, and input data sampled on falling edge of XCK. and when it is set to 1, this will be reversed. |



#### `UMSEL` combination

| UMSEL1 | UMSEL0 | Result Mode  |
| ------ | ------ | ------------ |
| 0      | 0      | Asynchronous |
| 0      | 1      | Synchronous  |
| 1      | 0      | Reserved     |
| 1      | 1      | Master SPI   |



#### `UPM` combination

| UPM1 | UPM2 | Result Mode          |
| ---- | ---- | -------------------- |
| 0    | 1    | Reserved             |
| 0    | 0    | Disabled             |
| 1    | 0    | Enabled; even parity |
| 1    | 1    | Enabled; odd parity  |



#### `UCSZ` combination

| UCSZ2 | UCSZ1 | UCSZ0 | Result data frame |
| :---: | :---: | :---: | :---------------: |
|   0   |   0   |   0   |       5-bit       |
|   0   |   0   |   1   |       6-bit       |
|   0   |   1   |   0   |       7-bit       |
|   0   |   1   |   1   |       8-bit       |
|   1   |   0   |   0   |     Reserved      |
|   1   |   0   |   1   |     Reserved      |
|   1   |   1   |   0   |     Reserved      |
|   1   |   1   |   1   |       9-bit       |



### `UBRR` bits

this register is used setup baud rate. it consists of 2 registers that operate as one, `UBRRH` and `UBRRL`.

each of these 2 registers has 8 bits. first 4 most important bits of `UBRRH` are reserved and do not participate in baud rate configuration. but the remaining 4 less important  bits combined with the bits of `UBRRL` will define the desired baud rate.

the formula to calculate the value for `UBRR` depend on the value of bit `U2X` would be as follow:

- when `U2X` is set
  - UBRR value = ( ( speed of crystal in `hz ` ) / (8 * desired baud rate) ) - 1
- when `U2X` is not set
  - UBRR value = ( ( speed of crystal in `hz ` ) / (16 * desired baud rate) ) - 1



for example if the desired baud rate is 9600 and crystal is 8 MHZ and `U2X` bit is not set, then the value of `UBRR` would be as follow:

51.083 = ( 8000000 / (16 * 9600 ) ) - 1

