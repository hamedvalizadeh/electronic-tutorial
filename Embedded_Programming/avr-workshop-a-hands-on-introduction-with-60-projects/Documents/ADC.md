# ADC Registers

there are 2 registers in `AVR` microcontrollers, that by the help of them we can setup and configure `ADC`.

- `ADMUX`
- `ADCSRA`



### ADMUX

this register is used to select the voltage source and pin of the `ADC`. it has following bits:

|        Bit        |   7   |   6   |   5   |  4   |  3   |  2   |  1   | 0    |
| :---------------: | :---: | :---: | :---: | :--: | :--: | :--: | :--: | ---- |
|     **Name**      | REFS1 | REFS2 | ADLAR |  -   | MUX3 | MUX2 | MUX1 | MUX0 |
| **Read / Write**  |  R/W  |  R/W  |  R/W  |  R   | R/W  | R/W  | R/W  | R/W  |
| **Initial Value** |   0   |   0   |   0   |  0   |  0   |  0   |  0   | 0    |



by setting `REFS1` and `REFS0` we can select which voltage source to be used by `ADC`. following table shows the combination of these bits rsult:

| REFS1 | REFS0 | Selection Result                                             |
| ----- | ----- | ------------------------------------------------------------ |
| 0     | 0     | AREF is used as VREF and internal VREF is turned off.        |
| 0     | 1     | AVCC with external capacitor on AREF is used as VREF.        |
| 1     | 0     | this combination is reserved by micro and should not be used. |
| 1     | 1     | Internal reference voltage of 2.56 is selected.              |



`ADLAR` bit is used to set the direction of 10 bits `ADC` result (that are filled after `ADC` has finished reading) in a 16 bits storage. there are two 8 bits storages named `ADCH` (ADC high) and `ADCL` (ADC low), that `ADC` result is set in them. as result is 10 bits, but storage is 16 bits, it is obvious that 6 bits are not used. so we can configure how to fill them (from left , or from right).

say the `ADC` result is as `1001001101`:

if we set this bit as `1`, the direction would be left aligned as follow:

|   Bit    |  15  |  14  |  13  |  12  |  11  |  10  |  9   |  8   |
| :------: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| **ADCH** |  1   |  0   |  0   |  1   |  0   |  0   |  1   |  1   |
| **Bit**  |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
| **ADCL** |  0   |  1   |  -   |  -   |  -   |  -   |  -   |  -   |

  

and set this bit as `0`, the direction would be right aligned as follow:

|   Bit    |  15  |  14  |  13  |  12  |  11  |  10  |  9   |  8   |
| :------: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| **ADCH** |  -   |  -   |  -   |  -   |  -   |  -   |  1   |  0   |
| **Bit**  |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
| **ADCL** |  0   |  1   |  0   |  0   |  1   |  1   |  0   |  1   |



**HINT:** after reading `ADCH` the `ADCL` would be cleared, so we should read `ADCL` first.



to select which `ADC` pin of the microcontroller to use, we should set `MUX` bits. these are a four bit number that indicates the decimal number of the `ADC` pin. for example if we want to use the `ADC5` of the `ATmega328p` we need to set `MUX` pins as follow:

```c
MUX3 = 0, MUX2 = 1, MUX1 = 0, MUX0 = 1
```

  

the following table would help to clarify the concept better:

| MUX3 | MUX2 | MUX1 | MUX0 | ADC Pin |
| ---- | ---- | ---- | ---- | ------- |
| 0    | 0    | 0    | 0    | ADC0    |
| 0    | 0    | 0    | 1    | ADC1    |
| 0    | 0    | 1    | 0    | ADC2    |
| 0    | 0    | 1    | 1    | ADC3    |
| 0    | 1    | 0    | 0    | ADC4    |
| 0    | 1    | 0    | 1    | ADC5    |
| 0    | 1    | 1    | 0    | ADC6    |
| 0    | 1    | 1    | 1    | ADC7    |

 



### ADCSRA

This register is responsible for enabling ADC, start ADC converting, pre-scaler selection and interrupt control.

this register contains the following bits:

|        Bit        |  7   |  6   |  5   |  4   |  3   |   2   |   1   |   0   |
| :---------------: | :--: | :--: | :--: | :--: | :--: | :---: | :---: | :---: |
|     **Name**      | ADEN | ADSC | ADFR | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0 |
| **Read / Write**  | R/W  | R/W  | R/W  | R/W  | R/W  |  R/W  |  R/W  |  R/W  |
| **Initial Value** |  0   |  0   |  0   |  0   |  0   |   0   |   0   |   0   |

**ADC Enable (ADEN):** ADC is turned on by setting this bit to logic 1 and turned off by writing logic 0.

**ADC Start Conversion (ADSC):** Setting this bit to logic 1 tells ADC to start the conversion. However, if Single conversion mode is selected, then we need to set this to logic 1 each time we need a conversion to start. Once the conversion is complete, the hardware sets it to zero. This can be considered as a flag to check if the conversion is complete.

**ADC Free Running Select (ADFR**): This bit can be used to toggle ADC into free running mode or single conversion mode. Setting it to 1 activates ADC in free running mode and clearing it sets it to single conversion mode.
**Note:** Some AVR microcontrollers have ADATE instead of ADFR. ADATE serves more complex purpose and used to enable Auto trigger by setting trigger select bits. Best way to know if your microcontroller uses ADFR or ADATE is through the datasheet, although you can verify the same from particular device header file. Higher end AVR versions may have additional ADCSRB register along with ADCSRA register.

**ADC Interrupt Flag (ADIF):** This is an interrupt bit which is set to 1 by hardware once conversion is complete and Data registers are updated. ADC conversion interrupt is executed if ADIE bit and I-bit in SREG is set. ADIE bit is Bit 3 in ADCSRA register and SREG is the status register. To avoid further confusion, status register contains result of the latest arithmetic instruction executed. I-bit is bit 7 in Status register and known as Global interrupt enable.
**Note:** `ADIF bit ` can be manually cleared by setting a 1 to the bit. Read it again, you are clearing this bit by writing 1 and not 0 as in the usual case. This is not an error in datasheet, but a well planned approach to clear a bit without disturbing other bits.

**ADC Interrupt Enable (ADIE)**: When this bit is written a 1 and the I-bit in SREG is set, the ADC Conversion Complete Interrupt is activated.

**ADC Pre-scaler Select Bits (ADPS)**: **ADPS2**, **ADPS1** and **ADPS0** bits are used to set circuit clock frequency. For ADC circuitry to work at its maximum resolution needs to be supplied with 50 kHz to 200 kHz frequency as per the datasheet; but the system clock will be generally higher (8 MHz, 10 MHz, 16 MHz etc). To reduce it to required frequency we use ADC pre-scaler bits. Suppose we have system clock with frequency 10Mhz (10000000 Hz) and set division factor to 64, then ADC clock frequency is 10000000/64 = 156250Hz = 156.25 `KHz`, which is between 50 to 200 `KHz` as mentioned in the datasheet.

The table below is the complete pre-scaler setting for these 3 bits **ADPS2**, **ADPS1** and **ADPS0**

| ADPS2 | ADPS1 | ADPS0 | Division Factor |
| :---: | :---: | :---: | :-------------: |
|   0   |   0   |   0   |        2        |
|   0   |   0   |   1   |        2        |
|   0   |   1   |   0   |        4        |
|   0   |   1   |   1   |        8        |
|   1   |   0   |   0   |       16        |
|   1   |   0   |   1   |       32        |
|   1   |   1   |   0   |       64        |
|   1   |   1   |   1   |       128       |