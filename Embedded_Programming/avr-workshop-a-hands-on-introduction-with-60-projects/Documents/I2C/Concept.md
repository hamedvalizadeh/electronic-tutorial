# What is it?

it is a communication method between embedded systems.



# Specification

it is based on master-slave communication method which can be organized with one ore many master connected with one or many slaves. many masters could share one or many slaves and many slaves could share one or many masters. but usually one master is communicating with one or many salves.

in `I2C` there are 2 wires that are connected between master and slaves, they are as follow:

- SDA wire: it stands for Serial Data Line; data is carried in this wire.
- SCL wire: it stands for Serial Clock Line; clock which is derived from master to slave to synchronize data transfer.



# Application

is used for short-distance communication with microcontrollers, microprocessors, LCDs, sensors (Gyroscope, etc.), memory devices (Flash, EEPROM) and is widely used in modern-day embedded hardware.

It's great for short-distance, low-speed communication **on the same PCB** or short cable.



# Basic Features

- **Master-slave** architecture
- Multiple **slaves**, one or more **masters**
- Devices identified by **7-bit (or 10-bit) addresses**
- Uses **pull-up resistors** on both lines (lines are open-drain)



# How It Works (Step by Step):

1. **Idle state**: Both SDA and SCL are high.
2. **Start condition**: Master pulls SDA low **while SCL is high** (signals the start of communication).
3. **Address phase**: Master sends a **7-bit address** + 1-bit **R/W bit** (0 = write, 1 = read).
4. **ACK/NACK**: Addressed slave pulls SDA low to **acknowledge**.
5. **Data phase**: Data is sent **one byte at a time**, with an ACK after each byte.
6. **Stop condition**: Master releases SDA high **while SCL is high** (signals end of transmission).



# Who Controls What?

- **Master** controls the clock (SCL), initiates communication.
- **Slaves** wait for their address and respond accordingly.
- Any device can **pull SDA low**, but **no one drives it high** (open-drain logic, pull-ups do that).



# Key Points:

- **Only two wires** needed
- **Multiple devices** on the same bus
- Speeds:
  - `Standard`
    Standard mode refers to the initial transfer speed mode of the I2C specification which allows up to **100 kbit/s**.
  - `Fast`
    Fast mode tightens several of the timing parameters to achieve **400 kbit/s** speed. Fast mode is widely supported by I2C slave devices, so a master may use it as long as it knows that the bus capacitance and pull-up strength allow it.
  - `Fast Mode** (FM+)`
    Fast mode plus supports up to **1 Mbit/s** using more powerful drivers and pull-ups to achieve faster rise and fall times. Fast mode plus devices are downward compatible with standard and fast mode devices.
  - `High Speed** (HS)`
    High speed mode of the I2C bus allows communication up to **3.4 Mbit/s**. In order to support high frequency HS mode master devices have a combination of an open-drain pull-down and current-source pull-up circuit on the SCL output.
  - `Ultra Fast`
    Ultra fast mode only supports one master, and it actively drives both clock and data lines at all times to achieve a **5 Mbit/s** transfer rate. Clock stretching, arbitration, read transfers, and acknowledgements are all omitted. It is mainly intended for applications where a transmission error would not cause catastrophic failure. This is also incompatible with other modes.



# Wire Length

The maximum practical length of an **I2C bus** in an **AVR-based** system (or any I2C system) depends on several factors, but here’s a quick summary:



### Typical Practical Lengths:

- **Standard I2C (100 kHz):** Up to **1 meter (3.3 feet)** is typically reliable.
- **Fast-mode I2C (400 kHz):** Usually safe up to **0.5 meters (1.6 feet)**.
- **Beyond 1 meter:** Unreliable without special techniques or hardware support.



I2C is not designed for long-distance communication. It relies on:

- **Open-drain lines** with **pull-up resistors**
- **Capacitance of the bus**, which slows down signal transitions

The I²C specification recommends a **maximum total bus capacitance of 400 pF**.

Cable length adds capacitance. For example:

- Standard ribbon cable: ~50–100 pF/m
- Shielded cable: even more



### How to Extend I2C Bus Length:

If you **must go longer** than ~1 meter:

1. **Lower the clock speed** (e.g., to 50 kHz or lower)
2. **Use stronger pull-up resistors** (e.g., 2.2kΩ instead of 4.7kΩ)
3. **Use bus buffers/extenders** like:
   - **P82B96**, **PCA9600**, **TCA9617A**
4. **Use differential I2C extenders** over CAT5/6 cables
5. **Consider using another protocol** like RS-485, CAN, or UART if longer distances are necessary