# Subject

in this article we try to introduce a general formula to calculate timer frequency and simplify the concept.



## General Formula

Yes — that’s a clean, systematic way to think about it.

**General idea:**

```
f_timer = f_clk / denominator

denominator = (timer clock prescaler) × (number of timer ticks per full PWM period)
```



| Mode (AVR name / typical)                       | ticks per full period          | When to use / notes                                          |
| ----------------------------------------------- | ------------------------------ | ------------------------------------------------------------ |
| **Fast PWM, fixed 8-bit (TOP = 0xFF)**          | 256                            | Timer0 in WGM 3 (WGM02=0,WGM01=1,WGM00=1) → counts 0..255    |
| **Fast PWM, fixed 16-bit (TOP = 0xFFFF)**       | 65536                          | 16-bit full-range (Timer1 fast PWM with TOP = 0xFFFF)        |
| **Fast PWM, TOP = OCRnA (variable TOP)**        | `1 + OCR`                      | Fast PWM where OCRnA defines TOP (common on Timer1 with OCR1A as TOP) |
| **Fast PWM, TOP = ICRn (variable TOP)**         | `1 + ICR`                      | Timer1 when using ICR1 as TOP (useful for custom freq)       |
| **Phase-Correct PWM, fixed 8-bit (TOP = 0xFF)** | `2 * 256` = 512                | Dual-slope (counts up then down) → frequency half of Fast PWM with same TOP |
| **Phase-Correct PWM, TOP = OCRnA**              | `2 * (1 + OCR)`                | Phase-correct variant with variable TOP                      |
| **Phase-Correct PWM, TOP = ICRn**               | `2 * (1 + ICR)`                | Typically Timer1 for precise phase-correct freq              |
| **CTC, toggle OC on compare (square wave)**     | `2 * (1 + OCR)`                | Toggle pin on every match → two matches per full output cycle |
| **CTC, non-toggle (match triggers event only)** | `1 + OCR`                      | Use for timed events; output pin behaviour depends on COM bits |
| **Normal mode (overflow) 8-bit**                | `1 + TOP` (TOP=0xFF) → 256     | Overflow when counter wraps from TOP to 0                    |
| **Normal mode (overflow) 16-bit**               | `1 + TOP` (TOP=0xFFFF) → 65536 | 16-bit overflow period                                       |