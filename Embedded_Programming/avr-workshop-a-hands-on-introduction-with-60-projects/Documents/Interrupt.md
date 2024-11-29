# What is Interrupt

there are 2 types of interrupts:

- external
- pin-change



interrupts could be triggered by one of the following changes in state of the pin:

- low level: when the state became grand. this cause the interrupt function continuously repeats until the state change to high.
- falling edge: occurs one time when the state changes from high to low.
- rising edge: occurs one time when the state changes from low to high.
- logic change: occurs one time when-ever the state changes in any direction.  



# External Interrupt

external interrupt in `ATmega328p` is raised by 2 pins named `INT0` (pin 4 or `PD2`) and `INT1` (pin 5 or `PD3`).



to config interrupt on any pins `INT0` or `INT1` we need to setup following registers:

- EICRA
- EIMSK
- EIFR



# EICRA

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



# EIMSK Register

it stands for `external interrupt mask`, and consists of the following bits:

| Bit Name      | -    | -    | -    | -    | -    | -    | INT1 | INT0 |
| ------------- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| Read/Write    | R    | R    | R    | R    | R    | R    | R/W  | R/W  |
| initial value | 0    | 0    | 0    | 0    | 0    | 0    | 0    | 0    |



this register is used to turn on interrupt function on each of external interrupt pins `INT1`, and `INT0`.



**HINT:** after enabling external interrupt by setting external registers, it is necessary to call function `sei` in `main` function to enable global interrupt register. 

**HITT:** to clear global interrupt register we can call `cli` function.     

**HITT:** a function named `ISR`will be called when .     



# EIFR Register

it stands for `external interrupt frag register`, and consists of the following bits:

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

  
