# Timer0 PWM on ATmega328P

- **Timer0** has **two PWM outputs**:
  - **OC0A (PD6)** → controlled by `OCR0A`
  - **OC0B (PD5)** → controlled by `OCR0B`
- Both share the **same counter (TCNT0)** and **same TOP value** (which defines PWM frequency).
- Each can have its **own duty cycle**, set by its compare register.



### Registers

##### Timer/Counter Control Register 0 A

|            | 7 bit  | 6 bit  | 5 bit  | 4 bit  | 3 bit | 2 bit | 1 bit | 0 bit |
| ---------- | ------ | ------ | ------ | ------ | ----- | ----- | ----- | ----- |
| **TCCR0A** | COM0A1 | COM0A0 | COM0B1 | COM0B0 | -     | -     | WGM01 | WGM00 |



##### Timer/Counter Control Register 0 B

|            | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| ---------- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| **TCCR0B** | FOC0A | FOC0B | -     | -     | WGM02 | CS02  | CS01  | CS00  |



##### Waveform Generator Mode bits

| MODE | WGM02 | WGM01 | WGM00 | DESCRIPTION               | TOP   |
| ---- | ----- | ----- | ----- | ------------------------- | ----- |
| 0    | 0     | 0     | 0     | Normal                    | 0xFF  |
| 1    | 0     | 0     | 1     | PWM, Phase Corrected      | 0xFF  |
| 2    | 0     | 1     | 0     | CTC                       | OCR0A |
| 3    | 0     | 1     | 1     | Fast PWM                  | 0xFF  |
| 4    | 1     | 0     | 0     | Reserved                  | -     |
| 5    | 1     | 0     | 1     | Fast PWM, Phase Corrected | OCR0A |
| 6    | 1     | 1     | 0     | Reserved                  | -     |
| 7    | 1     | 1     | 1     | Fast PWM                  | OCR0A |



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

following tables are similar for `Compare Output Mode B`.



following table is for non-PWM mode:

| COM0A1 | COM0A0 |                 Description                 |
| :----: | :----: | :-----------------------------------------: |
|   0    |   0    | normal port operation. OC0A is disconnected |
|   0    |   1    |        toggle OC0A on compare match         |
|   1    |   0    |         clear OC0A on compare match         |
|   1    |   1    |          set OC0A on compare match          |



following table is for Fast PWM mode:

| COM0A1 | COM0A0 |                         Description                          |
| :----: | :----: | :----------------------------------------------------------: |
|   0    |   0    |         normal port operation. OC0A is disconnected          |
|   0    |   1    | **WGM02=0 =>** normal port operation. OC0A is disconnected. **WGM02=1 =>** toggle OC0A on compare match |
|   1    |   0    | clear OC0A on compare match, set OC0A at BOTTOM => **non-inverting mode** |
|   1    |   1    | set OC0A on compare match, clear OC0A at BOTTOM => **inverting mode** |



following table is for phase correct PWM mode:

| COM0A1 | COM0A0 |                         Description                          |
| :----: | :----: | :----------------------------------------------------------: |
|   0    |   0    |         normal port operation. OC0A is disconnected          |
|   0    |   1    | **WGM02=0 =>** normal port operation. OC0A is disconnected. **WGM02=1 =>** toggle OC0A on compare match |
|   1    |   0    | clear OC0A on compare match when up-counting, set OC0A on compare match when down-counting |
|   1    |   1    | set OC0A on compare match when up-counting, clear OC0A on compare match when down-counting |



##### CS bits Settings

| CS02 | CS01 | CS00 | DESCRIPTION                                            |
| ---- | ---- | ---- | ------------------------------------------------------ |
| 0    | 0    | 0    | Timer/Counter0 Disabled                                |
| 0    | 0    | 1    | No Pre-scaling                                         |
| 0    | 1    | 0    | Clock / 8                                              |
| 0    | 1    | 1    | Clock / 64                                             |
| 1    | 0    | 0    | Clock / 256                                            |
| 1    | 0    | 1    | Clock / 1024                                           |
| 1    | 1    | 0    | External clock source on T0 pin, Clock on Falling edge |
| 1    | 1    | 1    | External clock source on T0 pin, Clock on rising edge  |



##### Timer/Counter Interrupt Mask Register

|            | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit  | 1 bit  | 0 bit |
| ---------- | ----- | ----- | ----- | ----- | ----- | ------ | ------ | ----- |
| **TIMSK0** | -     | -     | -     | -     | -     | OCIE0B | OCIE0A | TOIE0 |



##### Timer/Counter Interrupt Flag Register

|           | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| --------- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| **TIFR0** | -     | -     | -     | -     | -     | OCF0B | OCF0A | TOV0  |



##### Timer/Counter Register (stores the counter value)

|           | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| --------- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| **TCNT0** |       |       |       |       |       |       |       |       |



##### Output Compare Register

|           | 7 bit | 6 bit | 5 bit | 4 bit | 3 bit | 2 bit | 1 bit | 0 bit |
| --------- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| **OCR0A** |       |       |       |       |       |       |       |       |





---



### When both PWM outputs work (OC0A + OC0B)

- Works in **Fast PWM** or **Phase Correct PWM** modes where **TOP = 0xFF** (fixed).
- Both outputs generate PWM at the **same frequency**.
- Duty cycles are set independently (`OCR0A`, `OCR0B`).



### When only OC0B works

- In modes where **TOP = OCR0A** (Fast PWM or Phase Correct with variable TOP):
  - `OCR0A` defines the frequency (TOP).
  - The **OC0A pin is disabled** (can’t output PWM).
  - The **OC0B pin still works normally**.



### Practical meaning

| Mode                               | TOP   | OC0A PWM | OC0B PWM |
| ---------------------------------- | ----- | -------- | -------- |
| Fast / Phase Correct (TOP = 0xFF)  | 0xFF  | ✅        | ✅        |
| Fast / Phase Correct (TOP = OCR0A) | OCR0A | 🚫        | ✅        |



### Bottom line

You can have **two PWM outputs (OC0A + OC0B)** from **Timer0** only when **TOP = 0xFF** (the default 8-bit range).
 If you use **OCR0A as TOP**, **OC0A stops outputting PWM**, and only **OC0B** remains active.



# Sources

- https://avr-guide.github.io/timers-on-the-atmega328/
- https://courses.cs.washington.edu/courses/csep567/10wi/lectures/Lecture7.pdf