#### Generate PWM

in this microcontroller we have four pins capable of PWM, that are PB0, PB1, PB3, and PB4.

first config the direction of the desired pin as output by the help of `DDRB` register and bits `PORTB0`, `PORTB1`, `PORTB3`, and `PORTB4`.

then set the timer mode as `FAST`, or `Phase Correct`:

- in timer0 it could be done by the help of registers `TCCR0A` and `TCCR0B` and bits `WGM00`, `WGM01`, and `WGM02`.

- in timer1 it could be done by the help of register `TCCR1` and bits `PWM1B` and `PWM1A`.



now set the pre-scaler of the chosen timer:

- in timer1 we can set `CS10`, `CS11`, `CS12`, and `CS13` bits of register `TCCR1`.
- in timer0 we can `CS00`, `CS01`, and `CS02` bits of register `TCCR0B`.



this is the time to config the comparer:

- in timer0 we can do it by the help of bits `COM0A0`, `COM0A1`, `COM0B0`, and `COM0B1` of the register `TCCR0A`.
- in timer1 we can set bits `COM1B0`, and `COM1B1` of register `GTCCR`.



finally it is time to set the amount of comparer. based of the chosen port it can be set as follow:

- for port `PB4` we have `OCR1B` register.
- for port `PB0` we have `OCR0A` register.
- for port `PB1` we have `OCR0B` register.



#### Example Of ATTiny85 PWM

- PWM on port PB4: https://github.com/hamedvalizadeh/electronic-tutorial/blob/master/Embedded_Programming/avr-workshop-a-hands-on-introduction-with-60-projects/Code/31_PWM_ATTiny85_PB4/main.c
- PWM on port PB0: https://github.com/hamedvalizadeh/electronic-tutorial/blob/master/Embedded_Programming/avr-workshop-a-hands-on-introduction-with-60-projects/Code/31_PWM_ATTiny85_PB0/main.c
- PWM on port PB1: https://github.com/hamedvalizadeh/electronic-tutorial/blob/master/Embedded_Programming/avr-workshop-a-hands-on-introduction-with-60-projects/Code/31_PWM_ATTiny85_PB1/main.c
- PWM on ports PB0, PB1, and PB4: https://github.com/hamedvalizadeh/electronic-tutorial/blob/master/Embedded_Programming/avr-workshop-a-hands-on-introduction-with-60-projects/Code/31_PWM_ATTiny85_PB0_PB1_PB4/main.c

