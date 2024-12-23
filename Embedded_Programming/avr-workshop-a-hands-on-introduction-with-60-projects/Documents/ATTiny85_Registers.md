- Register Summary
  - [DDRB - Port B Data Direction Register](https://evan.widloski.com/notes/attiny/#ddrb---port-b-data-direction-register)
  - [PORTB - Port B Data Register](https://evan.widloski.com/notes/attiny/#portb---port-b-data-register)
- Timers
  - [TCCR0A - Timer 0 Counter Control Register A](https://evan.widloski.com/notes/attiny/#tccr0a---timer-0-counter-control-register-a)
  - [TCCR0B - Timer 0 Counter Control Register B](https://evan.widloski.com/notes/attiny/#tccr0b---timer-0-counter-control-register-b)
  - [TCNT0 - Timer 0 Counter Register](https://evan.widloski.com/notes/attiny/#tcnt0---timer-0-counter-register)
  - [0CR0A - Timer 0 Output Compare Register A](https://evan.widloski.com/notes/attiny/#cr0a---timer-0-output-compare-register-a)
  - [0CR0B - Timer 0 Output Compare Register B](https://evan.widloski.com/notes/attiny/#cr0b---timer-0-output-compare-register-b)
  - [TCCR1 - Timer 1 Counter Control Register](https://evan.widloski.com/notes/attiny/#tccr1---timer-1-counter-control-register)
  - TIMSK - Timer 0 Interrupt Mask Register
    - [Wave Form Generation](https://evan.widloski.com/notes/attiny/#wave-form-generation)
  - [TCCR1 - Timer 1 Counter Control Register](https://evan.widloski.com/notes/attiny/#tccr1---timer-1-counter-control-register-1)
  - Timer modes
    - [Normal](https://evan.widloski.com/notes/attiny/#normal)
    - [CTC - Clear Timer to Compare Match](https://evan.widloski.com/notes/attiny/#ctc---clear-timer-to-compare-match)
    - [Fast PWM](https://evan.widloski.com/notes/attiny/#fast-pwm)
    - [Waveform Generation](https://evan.widloski.com/notes/attiny/#waveform-generation)
- Interrupts
  - [TIMSK - Timer Interrupt Mask Register](https://evan.widloski.com/notes/attiny/#timsk---timer-interrupt-mask-register)
  - Hardware interrupts
    - [External interrupts](https://evan.widloski.com/notes/attiny/#external-interrupts)
    - [Pin change interrupt](https://evan.widloski.com/notes/attiny/#pin-change-interrupt)
  - [Timer interrupts](https://evan.widloski.com/notes/attiny/#timer-interrupts)
- ADC
  - [ADMUX - ADC Multiplexer Select](https://evan.widloski.com/notes/attiny/#admux---adc-multiplexer-select)
  - [ADCSRA - ADC Control and Status](https://evan.widloski.com/notes/attiny/#adcsra---adc-control-and-status)
  - [ADCSRB - ADC Control and Status](https://evan.widloski.com/notes/attiny/#adcsrb---adc-control-and-status)
  - [Manual Triggering](https://evan.widloski.com/notes/attiny/#manual-triggering)
  - [Auto triggering](https://evan.widloski.com/notes/attiny/#auto-triggering)

[![img](https://evan.widloski.com/notes/attiny/attiny85.jpg)](https://evan.widloski.com/notes/attiny/attiny85.jpg)

# Register Summary

## DDRB - Port B Data Direction Register

| DDB5 | DDB4 | DDB3 | DDB2 | DDB1 | DDB0 |
| :--: | :--: | :--: | :--: | :--: | :--: |
|      |      |      |      |      |      |

## PORTB - Port B Data Register

| PORTB5 | PORTB4 | PORTB3 | PORTB2 | PORTB1 | PORTB0 |
| :----: | :----: | :----: | :----: | :----: | :----: |
|        |        |        |        |        |        |

# Timers

## TCCR0A - Timer 0 Counter Control Register A

| COM0A1 | COM0A0 | COM0B1 | COM0B0 |      |      | WGM01 | WGM00 |
| :----: | :----: | :----: | :----: | :--: | :--: | :---: | :---: |
|        |        |        |        |      |      |       |       |

**COM0A[1:0]** - control behavior on OC0A compare match

**COM0B[1:0]** - control behavior on OC0B compare match

| COM0x1 | COM0x0 | OC0x behavior on compare match in Normal mode |
| :----: | :----: | :-------------------------------------------: |
|   0    |   0    |               OC0x disconnected               |
|   0    |   1    |                  Toggle OC0x                  |
|   1    |   0    |                  Clear OC0x                   |
|   1    |   1    |                   Set OC0x                    |

**WGM0[2:0]** - control bits for waveform generation mode. **WGM02** is in **TCCR0B**

| WGM02 | WGM01 | WGM00 |       Mode        |
| :---: | :---: | :---: | :---------------: |
|   0   |   0   |   0   |      Normal       |
|   0   |   0   |   1   | PWM Phase Correct |
|   0   |   1   |   0   |        CTC        |
|   0   |   1   |   1   |     Fast PWM      |

## TCCR0B - Timer 0 Counter Control Register B

| FOC0A | FOC0B |      |      | WGM02 | CS02 | CS01 | CS00 |
| :---: | :---: | :--: | :--: | :---: | :--: | :--: | :--: |
|       |       |      |      |       |      |      |      |

**FOC0A** -

**FOC0B** -

**WGM02** - part of waveform generation settings, see TCCR0A

**CS0[2:0]** - set Timer 0 prescaler

| CS02 | CS01 | CS00 |   Prescaler   |
| :--: | :--: | :--: | :-----------: |
|  0   |  0   |  0   | Timer stopped |
|  0   |  0   |  1   |      /1       |
|  0   |  1   |  0   |      /8       |
|  0   |  1   |  1   |      /64      |
|  1   |  0   |  0   |     /256      |
|  1   |  0   |  1   |     /102      |

## TCNT0 - Timer 0 Counter Register

Contains current counter value for Timer 0

## 0CR0A - Timer 0 Output Compare Register A

Compared against Timer 0 counter value to trigger some event, like an output compare interupt

## 0CR0B - Timer 0 Output Compare Register B

Compared against Timer 0 counter value to trigger some event, like an output compare interupt

## TCCR1 - Timer 1 Counter Control Register

| CTC1 | PWM1A | COM1A1 | COM1A0 | CS13 | CS12 | CS11 | CS10 |
| :--: | :---: | :----: | :----: | :--: | :--: | :--: | :--: |
|      |       |        |        |      |      |      |      |

**CTC1** - clear Timer 1 after compare match with OCR1C

**PWM1A** - enable PWM mode for OCR1A, (counter reset after compare match with OCR1C)

**COM1A[1:0]** - control OC1A behavior on compare match with OCR1A

| COM1A1 | COM1A0 |     Behavior      |
| :----: | :----: | :---------------: |
|   0    |   0    | OC1A disconnected |
|   0    |   1    |    Toggle OC1A    |
|   1    |   0    |    Clear OC1A     |
|   1    |   1    |     Set OC1A      |

**CS1[3:0]** - set Timer 1 prescaler

| CS13 | CS12 | CS11 | CS10 |   Prescaler   |
| :--: | :--: | :--: | :--: | :-----------: |
|  0   |  0   |  0   |  0   | Timer stopped |
|  0   |  0   |  0   |  1   |       1       |
|  0   |  0   |  1   |  0   |       2       |
|  0   |  0   |  1   |  1   |       4       |
|  0   |  1   |  0   |  0   |       8       |
|  0   |  1   |  0   |  1   |      16       |
|  0   |  1   |  1   |  0   |      32       |
|  0   |  1   |  1   |  1   |      64       |
|  1   |  0   |  0   |  0   |      128      |
|  1   |  0   |  0   |  1   |      256      |
|  1   |  0   |  1   |  0   |      512      |
|  1   |  0   |  1   |  1   |     1024      |
|  1   |  1   |  0   |  0   |     2048      |
|  1   |  1   |  0   |  1   |     4096      |
|  1   |  1   |  1   |  0   |     8192      |
|  1   |  1   |  1   |  1   |     16384     |

## TIMSK - Timer 0 Interrupt Mask Register

### Wave Form Generation

**TCNT0** - counter

**OCR0A** - stationary threshold

**OC0A** - output from compare

**OCR0B** - stationary threshold

**OC0B** - output from compare

## TCCR1 - Timer 1 Counter Control Register

| CTC1 | PWM1A | COM1A1 | COM1A0 | CS13 | CS12 | CS11 | CS10 |
| :--: | :---: | :----: | :----: | :--: | :--: | :--: | :--: |
|      |       |        |        |      |      |      |      |

**CTC1** - clear TCNT1 on compare match with OCR1C

**PWM1A** - enable PWM mode using OCR1A, reset on compare match with OCR1C

**COM1A1/COM1A0** - determine output behavior of OC1A on compare match with OCR1A

| COM1A1 | COM1A0 |               Mode                |
| :----: | :----: | :-------------------------------: |
|   0    |   0    | Comparator A disconnect from OC1A |
|   0    |   1    |            Toggle OC1A            |
|   1    |   0    |            Clear 0C1A             |
|   1    |   1    |             Set OC1A              |

**TCNT1** - counter

## Timer modes

**BOTTOM** - always 0

**MAX** - size of the counter

**TOP** - can be either **MAX** or OCRxx, depending on mode

### Normal

Timer counts up to TOP=0xFF then overflows and resets. Use OCRxx to either toggle output (OCxx) or trigger an interrupt

### CTC - Clear Timer to Compare Match

Timer is cleared when TCNT* (timer count) passes 0CR* (output compare)

### Fast PWM

Timer counts up to TOP then resets to BOTTOM.

### Waveform Generation

# Interrupts

**sei()** - enable interrupts

## TIMSK - Timer Interrupt Mask Register

**0CIE1A** - call TIMER1\COMPA\vect compare match

**0CIE1B** - call TIMER1\COMPB\vect compare match

**0CIE0A** - call TIMER0\COMPA\vect compare match

**0CIE0B** - call TIMER0\COMPB\vect compare match

**TOIE1** - call TIMER1\OVF\vect on timer overflow

**TOIE0** - call TIMER0\OVF\vect on timer overflow

## Hardware interrupts

### External interrupts

Only available to certain pins (only INT0 on attiny85) Interrupts must be unmasked (enabled)

**GIMSK** - set INT0 to enable interrupt

**MCUCR** - set ISC01/ISC00 to interrupt on change/rising/falling edge

```
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
  DDRB |= 1 << PB1;             // enable output on PB1

  GIMSK = 1<<INT0;              // unmask INT0 external interrupt
  MCUCR = 0<<ISC01 | 1<<ISC00;  // enable interrupt on rise/fall

  sei();                        // global interrupt enable

  while(1) {}
}

// interrupt service routine (ISR) for timer 0 A compare match
ISR(INT0_vect)
{
  PORTB ^= (1<<PB1);
}
```

### Pin change interrupt

These are still technically 'external' to the chip. Only one interrupt per port, but any pin on the port can trigger it. Interrupts must be unmasked (enabled)

**GIMSK** - set PCIE to unmask all PCI on port (only 1 port on attiny85)

**PCMSK** - set PCINTx to enable PCI on PCINT0 pin

```
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
  DDRB = 1 << PB1;  // enable output on PB1

  GIMSK = 1<<PCIE;   // turn on pin change interrupts
  PCMSK = 1<<PCINT0; // unmask PCINT0 pin change interrupt

  sei();             // global interrupt enable

  while(1) {}
}

// interrupt service routine for PCINT0
ISR(PCINT0_vect)
{
  PORTB ^= (1<<PB1);
}
```

## Timer interrupts

**TIMSK** - timer interrupt mask

**TIMER1\COMPA\vect** - call on compare match with OCR1A

**TIMER1\COMPB\vect** - call on compare match with OCR1B

**TIMER1\OVF\vect** - call when TOP is reached

```
#include <avr/io.h>
#include <avr/interrupt.h>

void main() {
    DDRB = (1<<PB1);  // set PB1 to output

    // enable timer clock and set prescaler to 1
    TCCR1 =  1<<CS10;

    // run TIMER1_COMPA interrupt when there's a match on timer 1, comparator A
    TIMSK |= 1<<OCIE1A;

    OCR1A = 255; // TOP - when to toggle OC1A
    OCR1C = 255; // when to reset TCINT1

    sei(); //enable interrupts

    while(1){}
}

ISR(TIMER1_COMPA_vect){
    PORTB ^= 1<<PB1; // toggle output
}
```

# ADC

## ADMUX - ADC Multiplexer Select

| REFS1 | REFS0 | ADLAR | REFS2 | MUX3 | MUX2 | MUX1 | MUX0 |
| :---: | :---: | :---: | :---: | :--: | :--: | :--: | :--: |
|       |       |       |       |      |      |      |      |

**REFS[2:0]** - used to set voltage reference for ADC

**ADLAR** - enable left adjust for result (ADH contains 8 MSBs of ADC reading)

**MUX[3:0]** - set adc to use

| MUX3 | MUX2 | MUX1 | MUX0 | Input Used |
| :--: | :--: | :--: | :--: | :--------: |
|  0   |  0   |  0   |  0   |    ADC0    |
|  0   |  0   |  0   |  1   |    ADC1    |
|  0   |  0   |  1   |  0   |    ADC2    |
|  0   |  0   |  1   |  1   |    ADC3    |

## ADCSRA - ADC Control and Status

| ADEN | ADSC | ADATE | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0 |
| :--: | :--: | :---: | :--: | :--: | :---: | :---: | :---: |
|      |      |       |      |      |       |       |       |

**ADEN** - enable ADC

**ADSC** - enable a single measurement in manual mode, or enable auto mode

**ADATE** - enable ADC auto triggering

**ADIF** - this output is set when the ADC measurement is ready. cleared automatically if ADC interrupt is used

**ADIE** - enable ADC interrupt ADCvect

**ADPS[2:0]** - adc prescaler

| ADPS2 | ADPS1 | ADPS0 | Prescaler |
| :---: | :---: | :---: | :-------: |
|   0   |   0   |   0   |     2     |
|   0   |   0   |   1   |     2     |
|   0   |   1   |   0   |     4     |
|   0   |   1   |   1   |     8     |
|   1   |   0   |   0   |    16     |
|   1   |   0   |   1   |    32     |
|   1   |   1   |   0   |    64     |
|   1   |   1   |   1   |    128    |

## ADCSRB - ADC Control and Status

| BIN  | ACME | IPR  |      |      | ADTS2 | ADTS1 | ADTS0 |
| :--: | :--: | :--: | :--: | :--: | :---: | :---: | :---: |
|      |      |      |      |      |       |       |       |

**BIN** - enables bipolar mode. 9 bit resolution + sign bit

**IPR** - reverse polarity in bipolar mode

**ADTS** - control ADC triggering in auto mode

| ADTS2 | ADTS1 | ADTS0 |     Trigger Source     |
| :---: | :---: | :---: | :--------------------: |
|   0   |   0   |   0   |      Free running      |
|   0   |   0   |   1   |   Analog comparator    |
|   0   |   1   |   0   |  External interrupt 0  |
|   0   |   1   |   1   | Timer0 Compare Match A |
|   1   |   0   |   0   |    Timer0 Overflow     |
|   1   |   0   |   1   | Timer0 Compare Match B |
|   1   |   1   |   1   |  Pin Change Interrupt  |

## Manual Triggering

## Auto triggering

**ADATE** - ADC auto trigger enable