# USART Hardware

in order to be able to connect an AVR micro controller we need one of the following devices:

- RS232 protocol cable.
  - although protocol has 9 pins, but we just need 3 of them; RX, TX, and GND pins. 
- USB to Serial connector. there are many types of this devices that some of them would be as follow:
  - CP2102
  - FT232RL
  - CH340



# Registers

there are 545465 registers in `AVR` microcontrollers, that by the help of them we can setup and configure `USART`.

- `UDR`

  - this is data register, that we can read data and write data to it. basically it consists of 2 resisters including `Tx` and `Rx`.

- `UCSRA`

  - USART Control and Status Register A. As the name suggests, is used for control and status flags.

- `UCSRB`

  - USART Control and Status Register A. As the name suggests, is used for control and status flags.

- `UCSRC`

  - USART Control and Status Register A. As the name suggests, is used for control and status flags.

- `UBRRL`

  - baud rate register. it is used to set the baud rate.

- `UBRRH`

  - baud rate register. it is used to set the baud rate.

  

### `UCSRA` Bits

| Bit Name | Description                                                  |
| -------- | ------------------------------------------------------------ |
| RXC      | receive complete. 1 is yes and means there is no more data to be read. it could be set to generate a receive complete interrupt. (the interrupt should be enabled by `RXCIE` bit in `UDCSRB` register) |
| TXC      | transfer is complete.  1 is yes and means there is no more data to be transmit. it could be set to generate a transmit complete interrupt. (the interrupt should be enabled by `TXCIE` bit in `UDCSRB` register) |
| UDRE     | is data register empty? 1 is yes and indicates that there the `UDR` register is ready to receive new data.  it could be set to generate a data register empty interrupt. (the interrupt should be enabled by `UDRIE` bit in `UDCSRB` register) |
| FE       | frame error.                                                 |
| DOR      | data overrun. if 1, it shows that buffer is full and a new character is waiting in the shift register. |
| PE       | parity error.                                                |
| U2X      | to double the transmission speed.                            |
| MPCM     | multi-process communication mode.                            |



### `UCSRB` Bits

| Bit Name | Description                                                  |
| -------- | ------------------------------------------------------------ |
| RXCIE    | RX Complete Interrupt Enable. 1 enable interrupt on RXC.     |
| TXCIE    | TX Complete Interrupt Enable. 1 enable interrupt on TXC.     |
| UDRIE    | data register empty Interrupt Enable. 1 enable interrupt on UDRE. |
| RCEN     | Receiver Enable. 1 means enabled.                            |
| TXEN     | Transmitter Enable. i means enabled.                         |
| UCSZ2    | by combining this bit with `UCSZ1` and `UCSZ0` bits in `UCSRC` register, we can set the number of data bits (character size ) in a frame the receiver and transmitter use. |
| RXB8     | Receive Data Bit 8                                           |
| TXB8     | Transmit Data Bit 8                                          |

