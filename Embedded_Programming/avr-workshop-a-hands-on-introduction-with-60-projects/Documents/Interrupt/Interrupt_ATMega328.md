# Basic External Interrupt Example

the following code is a simple example of enabling external interrupt on `INT0` (pin 4 or `PD2`) of the `ATmega328p`:

```c
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect)
{
    //your code ...
    EIFR &= ~(1 << 0);
}

void setup_int0()
{
    // turn on interrupt on pin 4 (INT0, PD2)
    EIMSK |= (1 << INT0);

    // st the trigger on rising edge
    EICRA |= (1 << ISC01) | (1 << ISC00);

    // enable global interrupt flag
    sei();

    // set the PD2 as input;
    DDRD &= ~(1 << PORTD2);
}

int main(void)
{
    setup_int0();
    
    //your code ...
        
    return 0;
}
```



# What is Interrupt

there are 2 types of interrupts:

- external
- pin-change



# External Interrupt

external interrupt in `ATmega328p` is raised by 2 pins named `INT0` (pin 4 or `PD2`) and `INT1` (pin 5 or `PD3`).



external interrupts could be triggered by one of the following changes in state of the pin:

- low level: when the state became grand. this cause the interrupt function continuously repeats until the state change to high.
- falling edge: occurs one time when the state changes from high to low.
- rising edge: occurs one time when the state changes from low to high.
- logic change: occurs one time when-ever the state changes in any direction.  



to config interrupt on any pins `INT0` or `INT1` we need to setup following registers:

- EICRA
- EIMSK
- EIFR



### EICRA

it stands for `external interrupt control register A`, and consists of the following bits:

|   Bit Name    | -    | -    | -    | -    | ISC11 | ISC10 | ISC01 | ISC00 |
| :-----------: | ---- | ---- | ---- | ---- | ----- | ----- | ----- | ----- |
|  Read/Write   | R    | R    | R    | R    | R/W   | R/W   | R/W   | R/W   |
| Initial Value | 0    | 0    | 0    | 0    | 0     | 0     | 0     | 0     |



this register is used to set the trigger type on `INT1` and `INT0` pins; bits `ISC11` and `ISC10` are to set the trigger type on `INT1`, and bits `ISC01` and `ISC00` are to set the trigger type on `INT0` as follow:

| ISC11 | ISC10 | Trigger Type of INT1 |
| :---: | :---: | :------------------: |
|   0   |   0   |      low level       |
|   0   |   1   |      any logic       |
|   1   |   0   |       falling        |
|   1   |   1   |        rising        |

 

| ISC01 | ISC00 | Trigger Type of INT0 |
| :---: | :---: | :------------------: |
|   0   |   0   |      low level       |
|   0   |   1   |      any logic       |
|   1   |   0   |       falling        |
|   1   |   1   |        rising        |



### EIMSK Register

it stands for `external interrupt mask`, and consists of the following bits:

|   Bit Name    |  -   |  -   |  -   |  -   |  -   |  -   | INT1 | INT0 |
| :-----------: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
|  Read/Write   |  R   |  R   |  R   |  R   |  R   |  R   | R/W  | R/W  |
| initial value |  0   |  0   |  0   |  0   |  0   |  0   |  0   |  0   |



this register is used to turn on interrupt function on each of external interrupt pins `INT1`, and `INT0`.

​     

**HITT:** a function named `ISR`will be called when the interrupt occurs.     



### EIFR Register

it stands for `external interrupt flag register`, and consists of the following bits:

| Bit Name      | -    | -    | -    | -    | -    | -    | INTF1 | INTF0 |
| ------------- | ---- | ---- | ---- | ---- | ---- | ---- | ----- | ----- |
| Read/Write    | R    | R    | R    | R    | R    | R    | R/W   | R/W   |
| initial value | 0    | 0    | 0    | 0    | 0    | 0    | 0     | 0     |



this register is used to say to the microcontroller that the execution of the interrupt function is completed by setting the value of bit `INTF1` or `INTF0` to 1.

when a trigger is raised for each of the external pins, corresponding bit of this register is set to 1 automatically.

Flag is cleared automatically when interrupt routine is executed.

Flag can be cleared by writing a 1 to it.



```c
EIFR &= ~(1<<0); //to say interrupt function of INT0 is completed

EIFR &= ~(1<<1); //to say interrupt function of INT1 is completed
```



to reset this register, best place would be the end of the `ISR` function (interrupt function).   

  

# Pin-Change Interrupt

there are 23 pins in `ATmega328p` that could be configured to trigger an interrupt on signal change.

these pins are not capable to distinguish the change type, they only will trigger on any changes in signal.

these pins are grouped to three groups that are known as banks:

- Bank 0: it consists of 8 pins that includes PCINT0 to PCINT7
- Bank 1: it consists of 7 pins that includes PCINT8 to PCINT14 (PCINT15 does not exists)
- Bank 2: it consists of 8 pins that includes PCINT16 to PCINT23



there is no individual `ISR` function for each `PCINT` pins, whereas there is one `ISR` function corresponding to each group.



**HINT:** in the `ISR` function it is our duty to find out the raised trigger is caused by which one of `PCINT` pins.



to config interrupt on any `PCINT` pins, we need to setup following registers:

- PCICR
- PCMSK0
- PCMSK1
- PCMSK2
- PCIFR



### PCICR register

it stands for `Pin Change Interrupt Control Register`, and consists of the following bits:

|   Bit Name    |  -   |  -   |  -   |  -   |  -   | PCIE2 | PCIE1 | PCIE0 |
| :-----------: | :--: | :--: | :--: | :--: | :--: | :---: | :---: | :---: |
|  Read/Write   |  R   |  R   |  R   |  R   |  R   |  R/W  |  R/W  |  R/W  |
| initial value |  0   |  0   |  0   |  0   |  0   |   0   |   0   |   0   |



this register is used to enable interrupt on any pin-change interrupt banks.

bit `PCIE2` corresponds to the Bank 2, so setting it to 1 will enable interrupt on all pins in bank 2;

bit `PCIE1` corresponds to the Bank 1, so setting it to 1 will enable interrupt on all pins in bank 1;

bit `PCIE0` corresponds to the Bank 0, so setting it to 1 will enable interrupt on all pins in bank 0;



### PCMSK0 Register

it stands for `Pin Change Interrupt Mask 0`, and consists of the following bits:

|  Bit Number   |   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
| :-----------: | :----: | :----: | :----: | :----: | :----: | :----: | :----: | :----: |
|   Bit Name    | PCINT7 | PCINT6 | PCINT5 | PCINT4 | PCINT3 | PCINT2 | PCINT1 | PCINT0 |
|  Read/Write   |  R/W   |  R/W   |  R/W   |  R/W   |  R/W   |  R/W   |  R/W   |  R/W   |
| initial value |   0    |   0    |   0    |   0    |   0    |   0    |   0    |   0    |



although setting `PCIE0` will enable interrupt on bank 0, but we also need to enable interrupt on each required pins of the bank. this is done by the helping `PCMSK0` register.  



### PCMSK1 Register

it stands for `Pin Change Interrupt Mask 1`, and consists of the following bits:

|  Bit Number   |  7   |    6    |    5    |    4    |    3    |    2    |   1    |   0    |
| :-----------: | :--: | :-----: | :-----: | :-----: | :-----: | :-----: | :----: | :----: |
|   Bit Name    |  -   | PCINT14 | PCINT13 | PCINT12 | PCINT11 | PCINT10 | PCINT9 | PCINT8 |
|  Read/Write   | R/W  |   R/W   |   R/W   |   R/W   |   R/W   |   R/W   |  R/W   |  R/W   |
| initial value |  0   |    0    |    0    |    0    |    0    |    0    |   0    |   0    |



although setting `PCIE1` will enable interrupt on bank 1, but we also need to enable interrupt on each required pins of the bank. this is done by the helping `PCMSK1` register.  



### PCMSK2 Register

it stands for `Pin Change Interrupt Mask 2, and consists of the following bits:

|  Bit Number   |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
| :-----------: | :-----: | :-----: | :-----: | :-----: | :-----: | :-----: | :-----: | :-----: |
|   Bit Name    | PCINT23 | PCINT22 | PCINT21 | PCINT20 | PCINT19 | PCINT18 | PCINT17 | PCINT16 |
|  Read/Write   |   R/W   |   R/W   |   R/W   |   R/W   |   R/W   |   R/W   |   R/W   |   R/W   |
| initial value |    0    |    0    |    0    |    0    |    0    |    0    |    0    |    0    |



although setting `PCIE2` will enable interrupt on bank 2, but we also need to enable interrupt on each required pins of the bank. this is done by the helping `PCMSK2` register.  



### PCIFR Register

it stands for `Pin Change Interrupt Flag Register`, and consists of the following bits:

|   Bit Name    |  -   |  -   |  -   |  -   |  -   | PCIF2 | PCIF1 | PCIF0 |
| :-----------: | :--: | :--: | :--: | :--: | :--: | :---: | :---: | :---: |
|  Read/Write   |  R   |  R   |  R   |  R   |  R   |  R/W  |  R/W  |  R/W  |
| initial value |  0   |  0   |  0   |  0   |  0   |   0   |   0   |   0   |



bit `PCIF2` corresponds to the Bank 2.

bit `PCIF1` corresponds to the Bank 1.

bit `PCIF0` corresponds to the Bank 0.



# Global Interrupt Register

after configuring required registers and setting necessary bits,  we need to enable global interrupt in `SREG` register. to do so we should call  function `sei` in `main` function.

to clear global interrupt register we can call `cli` function.

