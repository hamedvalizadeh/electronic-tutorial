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

 