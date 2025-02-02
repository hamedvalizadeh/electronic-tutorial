# Normal Mode Example

following code will call interrupt function every time that the timer counter register (`TCNT`) reach to its maximum; this time will take 0.52428 second.

we use internal oscillator of `ATmega328p`, and set the timer1 pre-scalar to 8.

```c
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void setup_timer1_interrupt()
{
    // set prescalar to 8
    TCCR1B |= (1 << CS11);

    // enable timer counter overflow 1 interrupt
    TIMSK1 |= (1 << TOIE1);

    sei();
}

ISR(TIMER1_OVF_vect)
{
    PORTB |= (1 << PORTB1);
    _delay_ms(100);
    PORTB &= ~(1 << PORTB1);
}

int main(void)
{
    setup_timer1_interrupt();
    DDRB |= (1 << PORTB0);
    DDRB |= (1 << PORTB1);
    while (1)
    {
        PORTB |= (1 << PORTB0);
        _delay_ms(100);
        PORTB &= ~(1 << PORTB0);
        _delay_ms(100);
    };
    return 0;
}
```



# CTC Mode Example

following code will call interrupt function every 5 seconds; the interrupt is raised based on the comparison of `TCNT` register period count with the value of `OCR1A`; when-ever it reaches the value of `OCR1A`, then the interrupt function will be called.

we use internal oscillator of `ATmega328p`, and set the timer1 pre-scalar to 1024.

```c
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(TIMER1_COMPA_vect)
{
    TCNT1 = 0;
    PORTB |= (1 << PORTB1);
    _delay_ms(1000);
    PORTB &= ~(1 << PORTB1);
}

void setup_timer1_ctc_interrupt()
{
    // number of periods to watch for.
    // this is calculated as: OCR1A = round(Seconds / (1 / ( F/Prescalar ) ) ) + 1
    // OCR1A = round( 5 / ( 1 / ( 1000000 / 1024 ) ) ) + 1 = round( 5 / ( 1 / ( 976.5625 ) ) ) + 1
    // = round( 5 / ( 0.001024 ) ) + 1 = round( 4882.8125 ) + 1 = 4884
    OCR1A = 4884;

    // set prescalar to 1024
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    // TCCR1B |= (1 << CS12) | (1 << CS10);

    // enable ctc 1 A interrupt
    TIMSK1 |= (1 << OCIE1A);

    sei();
}

int main(void)
{
    setup_timer1_ctc_interrupt();

    DDRB |= (1 << PORTB0);
    DDRB |= (1 << PORTB1);
    while (1)
    {
        PORTB |= (1 << PORTB0);
        _delay_ms(100);
        PORTB &= ~(1 << PORTB0);
        _delay_ms(100);
    };

    return 0;
}
```





# Timers

in `ATmega328p` there are two 8 bit timers name d`TIMER0` and `TIMER2` that their counter register have maximum value of 255, and one 16 bit timer named `TIMER1` that its counter register has maximum value of 65535.



# TIMER1

this timer has following registers:

- TCCR1A
  - stands for `Timer/Counter Control register 1 A`;
- TCCR1B
  - stands for `Timer/Counter Control register 1 B`; this register together with `TCCR1A` are used to control the characteristics of `Timer1`, for example operating mode, per-scalar.  
- TCNT1H
  - stands for `Timer/Counter 1 High`.
- TCNT1L
  - stands for `Timer/Counter 1 Low`. the result of counting is stored in this and `TCNT1H` registers.
- TIMSK1
  - stands for `Timer Interrupt Mask 1`.
- TIFR1
  - stands for `Timer Interrupt flag register 1`.



## TCCR1A

Timer/Counter Control Register 1 A

|   Bit Name    | COM1A1 | COM1A0 | COM1B1 | COM1B0 |  -   |  -   | WGM11 | WGM10 |
| :-----------: | :----: | :----: | :----: | :----: | :--: | :--: | :---: | :---: |
|  Read/Write   |  R/W   |  R/W   |  R/W   |  R/W   |  R   |  R   |  R/W  |  R/W  |
| initial value |   0    |   0    |   0    |   0    |  0   |  0   |   0   |   0   |



### Compare Output Mode A

following tables are similar for `Compare Output Mode B` and `Compare Output Mode C`.



following table is for non-PWM mode:

| COM1A1 | COM1A0 |                 Description                 |
| :----: | :----: | :-----------------------------------------: |
|   0    |   0    | normal port operation. OC0A is disconnected |
|   0    |   1    |        toggle OC0A on compare match         |
|   1    |   0    |         clear OC0A on compare match         |
|   1    |   1    |          set OC0A on compare match          |



following table is for Fast PWM mode:

| COM1A1 | COM1A0 |                         Description                          |
| :----: | :----: | :----------------------------------------------------------: |
|   0    |   0    |         normal port operation. OC0A is disconnected          |
|   0    |   1    | **WGM13:0 = 9 or 11 =>** toggle OC1A on compare match, OC1B/OC1C disconnected (normal port operation).  for all other **WGM1** settings  normal port operation (OC1B/OC1C disconnected) |
|   1    |   0    | clear OC0A on compare match, set OC0A at BOTTOM => **non-inverting mode** |
|   1    |   1    | set OC0A on compare match, clear OC0A at BOTTOM => **inverting mode** |



following table is for phase correct PWM mode:

| COM1A1 | COM1A0 |                         Description                          |
| :----: | :----: | :----------------------------------------------------------: |
|   0    |   0    |         normal port operation. OC0A is disconnected          |
|   0    |   1    | **WGM13:0 = 9 or 11 =>** toggle OC1A on compare match,OC1B/OC1C disconnected (normal port operation).  for all other **WGM1** settings  normal port operation (OC1B/OC1C disconnected) |
|   1    |   0    | clear OC0A on compare match when up-counting, set OC0A on compare match when down-counting |
|   1    |   1    | set OC0A on compare match when up-counting, clear OC0A on compare match when down-counting |



## TCCR1B

Timer/Counter Control Register 1 B

|   Bit Name    | ICNC1 | ICES1 |  -   | WGM13 | WGM12 | CS12 | CS11 | CS10 |
| :-----------: | :---: | :---: | :--: | :---: | :---: | :--: | :--: | :--: |
|  Read/Write   |  R/W  |  R/W  |  R   |  R/W  |  R/W  | R/W  | R/W  | R/W  |
| initial value |   0   |   0   |  0   |   0   |   0   |  0   |  0   |  0   |



### Timer1 Mode

the mode is controlled by the help of bits `WGM13`, `WGM12`, `WGM11`, and `WGM10` as following:

| MODE | WGM13 | WGM12 | WGM11 | WGM10 | DESCRIPTION                        | TOP    |
| ---- | ----- | ----- | ----- | ----- | ---------------------------------- | ------ |
| 0    | 0     | 0     | 0     | 0     | Normal                             | 0xFFFF |
| 1    | 0     | 0     | 0     | 1     | PWM, Phase Corrected, 8bit         | 0x00FF |
| 2    | 0     | 0     | 1     | 0     | PWM, Phase Corrected, 9bit         | 0x01FF |
| 3    | 0     | 0     | 1     | 1     | PWM, Phase Corrected, 10bit        | 0x03FF |
| 4    | 0     | 1     | 0     | 0     | CTC                                | OCR1A  |
| 5    | 0     | 1     | 0     | 1     | Fast PWM, 8bit                     | 0x00FF |
| 6    | 0     | 1     | 1     | 0     | Fast PWM, 9bit                     | 0x01FF |
| 7    | 0     | 1     | 1     | 1     | Fast PWM, 10bit                    | 0x03FF |
| 8    | 1     | 0     | 0     | 0     | PWM, Phase and Frequency Corrected | ICR1   |
| 9    | 1     | 0     | 0     | 1     | PWM, Phase and Frequency Corrected | OCR1A  |
| 10   | 1     | 0     | 1     | 0     | PWM, Phase Correct                 | ICR1   |
| 11   | 1     | 0     | 1     | 1     | PWM, Phase Correct                 | OCR1A  |
| 12   | 1     | 1     | 0     | 0     | CTC                                | ICR1   |
| 13   | 1     | 1     | 0     | 1     | RESERVED                           |        |
| 14   | 1     | 1     | 1     | 0     | Fast PWM                           | ICR1   |
| 15   | 1     | 1     | 1     | 1     | Fast PWM                           | OCR1A  |



**HINT:** `WGM` stands for `waveform generation mode`.



### Timer1 Pre-Scalar

by the help of this value  we can config the speed of counting in each second. by dividing frequency of the `ATmega328p` by the value of the pre-scalar parameter, the speed of the timer will be configured.

```
S = F / P;

S: timer speed
F: ATmega328p frequency in HZ
P: pre-scalar value 
```

 

so if we use internal oscillator of `ATmega328p`, and our pre-scalar is 8, then the speed would be  125000 HZ.



to config pre-scalar value we can use `CS12`, `CS11`, and `CS10` bits as the following table:

| CS12 | CS11 | CS10 | Result                                                 |
| ---- | ---- | ---- | ------------------------------------------------------ |
| 0    | 0    | 0    | No Pre-scalar - Timer will not work                    |
| 0    | 0    | 1    | pre-scalar = 1                                         |
| 0    | 1    | 0    | pre-scalar = 8                                         |
| 0    | 1    | 1    | pre-scalar = 64                                        |
| 1    | 0    | 0    | pre-scalar = 256                                       |
| 1    | 0    | 1    | pre-scalar = 1024                                      |
| 1    | 1    | 0    | External Clock source on T1 pin. clock on falling edge |
| 1    | 1    | 1    | External Clock source on T1 pin. clock on rising edge  |

  

### Timer1 Period

this is a period in which the counter register will increment in it by 1.

```
T = 1 / S;

T: period in each second.
1: 1 second.
S: timer clock speed.
```



so if we use internal oscillator of `ATmega328p`, and our pre-scalar is 8, then the speed of timer would be  125000 HZ, and thus the period will be 0.000008 second. it means that in every 0.000008 second, the counter will increment by 1. and thus the maximum amount of `Time1` is 65535, it will takes 0.52428 second for this timer to reach to its max value and to overflow.



## TIMSK1

this register is used to enable interrupts on `Timer1`, and has the following bits:

| Bit Name      | -    | -    | ICIE1 | -    | -    | OCIE1B | OCIE1A | TOIE1 |
| ------------- | ---- | ---- | ----- | ---- | ---- | ------ | ------ | ----- |
| Read/Write    | R    | R    | R/W   | R    | R    | R/W    | R/W    | R/W   |
| initial value | 0    | 0    | 0     | 0    | 0    | 0      | 0      | 0     |

 



## TIFR1

Timer/Counter Interrupt Flag Register

|   Bit Name    |  -   |  -   | ICF1 |  -   |  -   | OCF1B | OCF1A | TOV1 |
| :-----------: | :--: | :--: | :--: | :--: | :--: | :---: | :---: | :--: |
|  Read/Write   |  R   |  R   | R/W  |  R   |  R   |  R/W  |  R/W  | R/W  |
| initial value |  0   |  0   |  0   |  0   |  0   |   0   |   0   |  0   |

In normal operation the `TOV1` (timer over flow flag) bit, will be set in the same timer clock cycle as the TCNT1H, and TCNT1L registers becomes zero. The `TOV1` Flag in this case behaves like a 17th bit, except that it is only set, not cleared.



## TCCR1C

Timer/Counter Control Register 1 C

|        | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ------ | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| TCCR1C | FOC1A | FOC1B | -     | -     | -     | -     | -     | -     |



## TCNT1

Timer/Counter Register (stores the counter value, 16 bit)

|        | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ------ | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| TCNT1H |       |       |       |       |       |       |       |       |
| TCNT1L |       |       |       |       |       |       |       |       |



## OCR1A

Output Compare Register A (stores the compare value, 16 bit)

|        | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ------ | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| OCR1AH |       |       |       |       |       |       |       |       |
| OCR1AL |       |       |       |       |       |       |       |       |



## ICR1

Input Compare Register (can be used to stores the compare value, 16 bit)

|      | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ---- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| ICR1 |       |       |       |       |       |       |       |       |
| ICR1 |       |       |       |       |       |       |       |       |



# Sources

- https://avr-guide.github.io/timers-on-the-atmega328/
- https://courses.cs.washington.edu/courses/csep567/10wi/lectures/Lecture7.pdf
- https://mil.ufl.edu/5666/handouts/AVR_PWM_Reg_Info_Summary.pdf
