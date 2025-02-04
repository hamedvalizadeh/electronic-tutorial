# Registers

### TCCR2A

Timer/Counter Control Register 2

|        | 7 bit  | 6 bit  | 5 bit  | 4 bit  | 3 bit | 2 bit | 1 bit | 0 bit |
| ------ | ------ | ------ | ------ | ------ | ----- | ----- | ----- | ----- |
| TCCR2A | COM2A1 | COM2A0 | COM2B1 | COM2B0 | -     | -     | WGM21 | WGM20 |



##### Config Timer Mode

Waveform Generation Mode Bit Description:

| Mode | WGM22 | WGM21 | WGM20 | Timer/Counter Mode of Operation | TOP  | Update of OCRx at | TOV FlagSet on(1)(2) |
| ---- | ----- | ----- | ----- | ------------------------------- | ---- | ----------------- | -------------------- |
| 0    | 0     | 0     | 0     | Normal                          | 0xFF | Immediate         | MAX                  |
| 1    | 0     | 0     | 1     | PWM, Phase Correct              | 0xFF | TOP               | BOTTOM               |
| 2    | 0     | 1     | 0     | CTC                             | OCRA | Immediate         | MAX                  |
| 3    | 0     | 1     | 1     | Fast PWM                        | 0xFF | BOTTOM            | MAX                  |
| 4    | 1     | 0     | 0     | Reserved                        | –    | –                 | –                    |
| 5    | 1     | 0     | 1     | PWM, Phase Correct              | OCRA | TOP               | BOTTOM               |
| 6    | 1     | 1     | 0     | Reserved                        | –    | –                 | –                    |
| 7    | 1     | 1     | 1     | Fast PWM                        | OCRA | BOTTOM            | TOP                  |



### TCCR2B

Timer/Counter Control Register 2

|        | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ------ | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| TCCR2B | FOC2A | FOC2B | -     | -     | WGM22 | CS22  | CS21  | CS20  |



in fast CTC mode, the output frequency of the connected pin, is calculated as follow:

```powershell
f_oc = f_clk / (2 * N * (1 + OCR) )

f_oc = frequency of connected pin
f_clk = frequency of microcontroller
N = pre-scaller of timer
OCR = value set for the related output compare register
```

 

in fast PWM mode, the output frequency of the connected pin, is calculated as follow:

```powershell
f_oc = f_clk / (N * 256)

f_oc = frequency of connected pin
f_clk = frequency of microcontroller
N = pre-scaller of timer
```

 

in Phase Corrected PWM mode, the output frequency of the connected pin, is calculated as follow:

```powershell
f_oc = f_clk / (N * 510)

f_oc = frequency of connected pin
f_clk = frequency of microcontroller
N = pre-scaller of timer
```

 

##### Compare Output Mode A

Compare Output Mode, non-PWM Mode

| COM2A1 | COM2A0 | Description                               |
| ------ | ------ | ----------------------------------------- |
| 0      | 0      | Normal port operation, OC0A disconnected. |
| 0      | 1      | Toggle OC2A on Compare Match              |
| 1      | 0      | Clear OC2A on Compare Match               |
| 1      | 1      | Set OC2A on Compare Match                 |



Compare Output Mode, Fast PWM Mode:

| COM2A1 | COM2A0 | Description                                                  |
| ------ | ------ | ------------------------------------------------------------ |
| 0      | 0      | Normal port operation, OC2A disconnected.                    |
| 0      | 1      | WGM22 = 0: Normal Port Operation, OC0A Disconnected. WGM22 = 1: Toggle OC2A on Compare Match. |
| 1      | 0      | Clear OC2A on Compare Match, set OC2A at BOTTOM, (non-inverting mode). |
| 1      | 1      | Set OC2A on Compare Match, clear OC2A at BOTTOM, (inverting mode). |



Compare Output Mode, Phase Correct PWM Mode:

| COM2A1 | COM2A0 | Description                                                  |
| ------ | ------ | ------------------------------------------------------------ |
| 0      | 0      | Normal port operation, OC2A disconnected.                    |
| 0      | 1      | WGM22 = 0: Normal Port Operation, OC2A Disconnected. WGM22 = 1: Toggle OC2A on Compare Match. |
| 1      | 0      | Clear OC2A on Compare Match when up-counting. Set OC2A on Compare Match when down-counting. |
| 1      | 1      | Set OC2A on Compare Match when up-counting. Clear OC2A on Compare Match when down-counting |



##### Compare Output Mode B

Compare Output Mode, non-PWM Mode:

| COM2B1 | COM2B0 | Description                               |
| ------ | ------ | ----------------------------------------- |
| 0      | 0      | Normal port operation, OC2B disconnected. |
| 0      | 1      | Toggle OC2B on Compare Match              |
| 1      | 0      | Clear OC2B on Compare Match               |
| 1      | 1      | Set OC2B on Compare Match                 |



Compare Output Mode, Fast PWM Mode:

| COM2B1 | COM2B0 | Description                                                  |
| ------ | ------ | ------------------------------------------------------------ |
| 0      | 0      | Normal port operation, OC2B disconnected.                    |
| 0      | 1      | Reserved                                                     |
| 1      | 0      | Clear OC2B on Compare Match, set OC2B at BOTTOM, (non-inverting mode). |
| 1      | 1      | Set OC2B on Compare Match, clear OC2B at BOTTOM, (inverting mode). |



Compare Output Mode, Phase Correct PWM Mode:

| COM2B1 | COM2B0 | Description                                                  |
| ------ | ------ | ------------------------------------------------------------ |
| 0      | 0      | Normal port operation, OC2B disconnected.                    |
| 0      | 1      | Reserved                                                     |
| 1      | 0      | Clear OC2B on Compare Match when up-counting. Set OC2B on Compare Match when down-counting. |
| 1      | 1      | Set OC2B on Compare Match when up-counting. Clear OC2B on Compare Match when down-counting. |



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



##### ASSR

Asynchronous Status Register

|          | 7 bit | 6 bit | 5 bit | 4 bit  | 3 bit   | 2 bit   | 1 bit   | 0 bit   |
| -------- | ----- | ----- | ----- | ------ | ------- | ------- | ------- | ------- |
| **ASSR** |       | EXCLK | AS2   | TCN2UB | OCR2AUB | OCR2BUB | TCR2AUB | TCR2BUB |



timer2 has 2 source of clock, one from microcontroller, and the other could be from an external clock. if bit `AS2` is not, set the clock source of the timer2 would be the one used for microcontroller, but if bit `AS2` is set as `1`, then the clock source of the timer2 would be the external crystal connected to the pins `TOSC1` and `TOSC2` of the microcontroller.

one of the most important use of timer2 is to create a real timing clock (`RTC`). when pre-scalar is set as 128, and an external crystal of 32678Hz (Clock Crystal) is connected to `TOSC1` and `TOSC2` pins, the frequency of timer2 clock would be exactly 256, which is equal to the maximum size of `TCNT2`, that means in each exact 1 second the `TCNT2` would overflow. based on this exact time we could use the interrupt overflow function to calculate the real time.





# Sources

- https://avr-guide.github.io/timers-on-the-atmega328/
- https://courses.cs.washington.edu/courses/csep567/10wi/lectures/Lecture7.pdf