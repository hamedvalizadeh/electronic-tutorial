# I2C Steps Description

To use `I²C`, follow these steps:

1. initialize `I2C` as master:
   1. enable TWI hardware by configuring  ports to change their default behavior (I/O Ports) to function as `SDA` and `SCL` ports.
   2. set the speed of data transfer (`I2C` clock (SCL) ), by configuring pre-scalar value and Baud Rate.
2. start communication: in this step we config the master to start the communication.
3. send address of slave: to tell to the master with which slave to talk. The address field consists of **7 bits for the slave address**, and **1 bit (LSB)** to indicate direction: `0` for write, `1` for read.
4. send data
5. stop communication: in this step we config the master to stop the communication.



# Registers List

there are following registers in AVR `ATMega328p` to setup and use `I2C` communication.

- TWBR
- TWSR
- TWCR
- TWAR
- TWAMR
- TWDR



# TWBR

it stands for TWI Baud Rate. 

as `I2C` is a clocked protocol, the master provides the clock.

by the help of this register and TWSR we can set the speed of  clock on`SCL`.

if the clock is too fast some slaves will not respond, and if too slow it will cause **inefficient** data transfer.

- this register has 8 bits that can be set from 0 to 255.
- it is used only in master mode.
- Lower values of `TWBR` result in faster I²C clock speeds, with `TWBR = 0` giving one of the highest speeds possible.



# TWSR

it stands for TWI Status Register. it has 8 bits.

this register is meant to show status of the data being transferred. there are bits `TWS7`, `TWS6`, `TWS5`, `TWS4`, `TWS3`. to get the status we need to mask the last three LSB bits as follow:

```c
uint8_t status = TWSR & 0xF8;

or 
    
uint8_t status = TWSR & 0b11111000;
```



| Status Purpose                       | Hex Code | TWSR (Bits 7:3) Value |
| ------------------------------------ | -------- | --------------------- |
| START condition transmitted          | `0x08`   | `00001`               |
| Repeated START transmitted           | `0x10`   | `00010`               |
| SLA+W transmitted, ACK received      | `0x18`   | `00011`               |
| SLA+W transmitted, NACK received     | `0x20`   | `00100`               |
| Data byte transmitted, ACK received  | `0x28`   | `00101`               |
| Data byte transmitted, NACK received | `0x30`   | `00110`               |
| Arbitration lost                     | `0x38`   | `00111`               |
| SLA+R transmitted, ACK received      | `0x40`   | `01000`               |
| SLA+R transmitted, NACK received     | `0x48`   | `01001`               |
| Data byte received, ACK returned     | `0x50`   | `01010`               |
| Data byte received, NACK returned    | `0x58`   | `01011`               |

**Caution:** the binary equivalent columns in the above 5 MSB bits of the register, and for the lats 3 LSB bits, consider 0. for example for Hex code `0x08` the precise binary would be `0b00001000`.



also it is used to set timing pre-scalar used to specify clock speed of `SCL`, by setting bits `TWPS1` and `TWPS0`. as follow:

| TWPS1 | TWPS0 | Pre-scaler |
| ----- | ----- | ---------- |
| 0     | 0     | 1          |
| 0     | 1     | 4          |
| 1     | 0     | 16         |
| 1     | 1     | 64         |

 

**Hint:** bit 2 is reserved



# Clock Speed Calculation

by setting `TWBR` register and pre-scalar value through `TWPS1` and `TWPS0` bits of`TWSR` register, we can set the clock speed of `I2C` in  by considering following formula:

```c
SCL = (Fcpu) / (16 + (2*TWBR*ps))

Fcpu = frequency of microcontroller
ps = pre-scalar
```



if `Fcpu = 16 MHz`, and `TWBR = 72`, and  `ps = 1 (TWPS1 = 0, TWPS0 = 0)`, then the clock speed of `I2C` will be `100 Khz`.



# TWCR

it stands for TWI control register, and is used to control the operation of `I2C`. some operations as follow:

- start the communication
- stop the communication
- enable `I2C` bus
- generate a receive acknowledge
- control bus collisions

  

##### Register Bits:

| Bit  | Name      | Purpose                                              |
| ---- | --------- | ---------------------------------------------------- |
| 7    | **TWINT** | TWI Interrupt Flag — clears when you start an action |
| 6    | **TWEA**  | TWI Enable Acknowledge — sends ACK after reception   |
| 5    | **TWSTA** | TWI START Condition — requests a START condition     |
| 4    | **TWSTO** | TWI STOP Condition — requests a STOP condition       |
| 3    | **TWWC**  | TWI Write Collision Flag — set if write collision    |
| 2    | **TWEN**  | TWI Enable — turns on the TWI hardware               |
| 1    | —         | Reserved                                             |
| 0    | **TWIE**  | TWI Interrupt Enable — enables TWI interrupt         |



##### TWINT

TWINT is set to 1 **by hardware** when a TWI operation is completed. It is **cleared by writing 1** to it. You must write 1 to clear it before the next operation.

to check if it is cleared, we can pull its value while it is 1, as follow:

```c
while (!(TWCR & (1 << TWINT)));  // Wait until operation is done
```

#####   

##### TWWC

it is read only and is set to 1 when writing to TWDR while previous data has not been transmitted yet.



# TWAR

it stands for TWI address register. 

In slave mode, bits 7–1 of `TWAR` hold the device’s 7-bit I²C address. Bit 0 (`TWGCE`) enables response to the General Call address (0x00). This register is **not used in master mode**.

**HINT:** what is general call. if master wants to broadcast a message to all its slaves, it sends 0x00 as slave address. this way any slave that has `TWGCE` set to 1, could receive the incoming data.



# TWAMR

Each bit in `TWAMR` determines whether the corresponding bit in the slave address (`TWAR`) must match exactly (0) or can be ignored (1). it means if one bit in this register is set to 1, then address match logic ignores that corresponding bit of `TWAR` with incoming bit address. 

**Hints:**

- this register only applies in **slave mode**.
- Default value is `0x00`, which means **no masking** - address must match exactly.

# TWDR

this is the main register that acts as an 8 bits buffer to send and receive data.

In master mode, data written to `TWDR` will be sent to the slave. In slave mode, `TWDR` holds either the data received from the master or the data to be sent back.



Example code in master mode to send data:

```c
// Master Mode – Sending data to slave
TWDR = 0x42;  // Load data byte into TWDR
TWCR = (1 << TWINT) | (1 << TWEN);  // Start transmission
while (!(TWCR & (1 << TWINT)));    // Wait for transmission to finish
```



Example code in master mode to receive data:

```c
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);  // Receive with ACK
while (!(TWCR & (1 << TWINT)));                  // Wait for reception
uint8_t data = TWDR;                             // Read received byte	
```



Example code in slave mode to receive data:

```c
if (TWSR == 0x80) {  // SLA+W received, data available
    uint8_t received = TWDR;
    TWDR = received;  // Prepare to send it back (echo)
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
}
```





# I2C Steps Codes

##### init

```c
void I2C_init(void) {
    // Set prescaler bits TWPS1 and TWPS0 in TWSR to 0 (Prescaler = 1)
    TWSR = 0x00;  // TWSR = (0<<TWPS1)|(0<<TWPS0);  // Use 1x prescaler

    // Set bit rate register TWBR to 72
    // Formula: SCL freq = F_CPU / (16 + 2 * TWBR * 4^TWPS)
    TWBR = 72;    // For 100kHz SCL when F_CPU = 16MHz and Prescaler = 1

    // Enable TWI (I²C) by setting TWEN in TWCR
    // TWCR = TWI Control Register
    // TWEN = TWI Enable Bit (must be set to enable TWI hardware)
    // TWINT = TWI Interrupt Flag (cleared by writing 1 to it)
    // Other bits default to 0 (no start, stop, ack yet)
    TWCR = (1 << TWEN);  // Enable TWI module
}
```



##### start

```c
void I2C_start(void) {
    // Set TWSTA to generate START condition
    // Set TWINT to clear interrupt flag and trigger action
    // Set TWEN to enable TWI hardware
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);  // START condition

    // Wait for TWINT to be set again (means START condition has been transmitted)
    while (!(TWCR & (1 << TWINT)));  // Wait for START to complete
}
```



##### send address

```c
void I2C_write_address(uint8_t address) {
    // Load slave address into TWDR (left-shift by 1 and set R/W bit = 0 for write)
    // LSB = 0 means Write operation
    TWDR = (address << 1);  // Load 7-bit address + write bit (0)

    // Clear TWINT to start transmission, keep TWEN enabled
    TWCR = (1 << TWEN) | (1 << TWINT);  // Start sending address

    // Wait for TWINT = 1 (transmission finished)
    while (!(TWCR & (1 << TWINT)));  // Wait for ACK/NACK from slave
}
```



##### send data

```c
void I2C_write_data(uint8_t data) {
    // Load data byte into TWDR (TWI Data Register)
    TWDR = data;

    // Clear TWINT to begin transmission, keep TWEN enabled
    TWCR = (1 << TWEN) | (1 << TWINT);  // Send data byte

    // Wait for TWINT = 1 (data transmission completed)
    while (!(TWCR & (1 << TWINT)));  // Wait for ACK/NACK
}
```



##### stop

```c
void I2C_stop(void) {
    // Set TWSTO to generate STOP condition
    // Set TWINT to clear the interrupt flag
    // Set TWEN to keep TWI enabled
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);  // STOP condition

    // NOTE: No need to wait for TWINT after STOP
    // Bus is released after STOP is sent
}
```

