# Generate PWM

in this microcontroller we have 6 pins capable of generating `PWM`. these are as follow:

| Port Name | Pin Number | Duty Cycle Register |
| --------- | ---------- | ------------------- |
| PD6       | 12         | OCR0A               |
| PD5       | 11         | OCR0B               |
| PB1       | 15         | OCR1A               |
| PB2       | 16         | OCR1B               |
| PB3       | 17         | OCR2A               |
| PD3       | 5          | OCR2B               |



first of all we need to config the direction of Port that we want to setup `PWM` on it.



then set the timer mode as `FAST`, or `Phase Correct`:

- in timer0 it could be done by the help of registers `TCCR0A` and `TCCR0B` and bits `WGM00`, `WGM01`, and `WGM02`.

- in timer1 it could be done by the help of bits `WGM10`, `WGM11` of register `TCCR1A` and bits `WGM12`, `WGM13` of register `TCCR1B`.



now set the pre-scaler of the chosen timer:

- in timer1 we can set `CS10`, `CS11`, and `CS12` bits of register `TCCR1B`.
- in timer0 we can `CS00`, `CS01`, and `CS02` bits of register `TCCR0B`.



this is the time to config the comparer:

- in timer0 we can do it by the help of bits `COM0A0`, `COM0A1`, `COM0B0`, and `COM0B1` of the register `TCCR0A`.
- in timer1 we can do it by the help of bits `COM1A0`, `COM1A1`, `COM1B0`, and `COM1B1` of the register `TCCR1A`.



finally it is time to set the amount of comparer register. based of the chosen port it can be set as follow:

- for port `PB1` we have `OCR1A` register.
- for port `PB2` we have `OCR1B` register.
- for port `PB3` we have `OCR2A` register.
- for port `PD3`we have `OCR2B` register.
- for port `PD5`we have `OCR0B` register.
- for port `PD6`we have `OCR0A` register.



# Example Codes

- PWM on PD6: https://github.com/hamedvalizadeh/electronic-tutorial/blob/master/Embedded_Programming/avr-workshop-a-hands-on-introduction-with-60-projects/Code/32_PWM_ATmega328p_PD6/main.c
- PWM on PB1: https://github.com/hamedvalizadeh/electronic-tutorial/blob/master/Embedded_Programming/avr-workshop-a-hands-on-introduction-with-60-projects/Code/32_PWM_ATmega328p_PB1/main.c
