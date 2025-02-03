# Registers

### TCCR2A

Timer/Counter Control Register 2

|        | 7 bit  | 6 bit  | 5 bit  | 4 bit  | 3 bit | 2 bit | 1 bit | 0 bit |
| ------ | ------ | ------ | ------ | ------ | ----- | ----- | ----- | ----- |
| TCCR2A | COM2A1 | COM2A0 | COM2B1 | COM2B0 | -     | -     | WGM21 | WGM20 |



##### Config Timer Mode

Waveform Generator Mode bits

| MODE | WGM21 | WGM20 | DESCRIPTION         | TOP  |
| ---- | ----- | ----- | ------------------- | ---- |
| 0    | 0     | 0     | Normal              | 0xFF |
| 1    | 0     | 1     | PWM Phase Corrected |      |
| 2    | 1     | 0     | CTC                 | OCR2 |
| 3    | 1     | 1     | Fast PWM            |      |



### TCCR2B

Timer/Counter Control Register 2

|        | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ------ | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| TCCR2B | FOC2A | FOC2B | -     | -     | WGM22 | CS22  | CS21  | CS20  |



##### Config pre-scaling

CS bits Settings

| CS22 | CS21 | CS20 | DESCRIPTION             |
| ---- | ---- | ---- | ----------------------- |
| 0    | 0    | 0    | Timer/Counter2 Disabled |
| 0    | 0    | 1    | No pre-scaling          |
| 0    | 1    | 0    | Clock / 8               |
| 0    | 1    | 1    | Clock / 32              |
| 1    | 0    | 0    | Clock / 64              |
| 1    | 0    | 1    | Clock / 128             |
| 1    | 1    | 0    | Clock / 256             |
| 1    | 1    | 1    | Clock / 1024            |



Timer/Counter Interrupt Mask Register 2

|        | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit  | 1 bit  | 0 bit |
| ------ | ----- | ----- | ----- | ----- | ----- | ------ | ------ | ----- |
| TIMSK2 | -     | -     | -     | -     | -     | OCIE2B | OCIE2A | TOIE2 |



Timer/Counter Interrupt Flag Register 2

|       | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| TIFR2 | -     | -     | -     | -     | -     | OCF2B | OCF2A | TOV2  |



Timer/Counter Register 2 (stores the counter value)

|       | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| TCNT2 |       |       |       |       |       |       |       |       |



Output Compare Register 2 (stores the compare value)

|      | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ---- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| OCR2 |       |       |       |       |       |       |       |       |





# Sources

- https://avr-guide.github.io/timers-on-the-atmega328/
- https://courses.cs.washington.edu/courses/csep567/10wi/lectures/Lecture7.pdf