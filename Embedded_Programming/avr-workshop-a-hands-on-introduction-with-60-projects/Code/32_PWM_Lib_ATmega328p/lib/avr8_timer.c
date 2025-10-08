#include "avr8_timer.h"

/*===================== TIMER0 =====================*/
void avr8_timer_0_init(const avr8_timer_0_config_t *cfg)
{
    // Set output pin
    switch (cfg->output)
    {
    case PWM0_OC0A:
        DDRD |= (1 << PD6);
        break;
    case PWM0_OC0B:
        DDRD |= (1 << PD5);
        break;
    }

    TCCR0A = 0;
    TCCR0B = 0;

    // Waveform generation
    switch (cfg->mode)
    {
    case PWM0_NORMAL:
        break;
    case PWM0_PHASE_CORRECT:
        TCCR0A |= (1 << WGM00);
        break;
    case PWM0_CTC:
        TCCR0A |= (1 << WGM01);
        break;
    case PWM0_FAST:
        TCCR0A |= (1 << WGM00) | (1 << WGM01);
        break;
    case PWM0_PHASE_CORRECT_OCR:
        TCCR0A |= (1 << WGM00);
        TCCR0B |= (1 << WGM02);
        break;
    case PWM0_FAST_OCR:
        TCCR0A |= (1 << WGM00) | (1 << WGM01);
        TCCR0B |= (1 << WGM02);
        break;
    }

    if (cfg->inverting)
    {
        if (cfg->output == PWM0_OC0A)
            TCCR0A |= (1 << COM0A1) | (1 << COM0A0); // Inverting
        else
            TCCR0A |= (1 << COM0B1) | (1 << COM0B0); // Inverting
    }
    else
    {
        if (cfg->output == PWM0_OC0A)
            TCCR0A |= (1 << COM0A1);
        else
            TCCR0A |= (1 << COM0B1);
    }

    avr8_timer_0_set_duty(cfg, cfg->duty);

    // Prescaler
    switch (cfg->prescaler)
    {
    case 1:
        TCCR0B |= (1 << CS00);
        break;
    case 8:
        TCCR0B |= (1 << CS01);
        break;
    case 64:
        TCCR0B |= (1 << CS01) | (1 << CS00);
        break;
    case 256:
        TCCR0B |= (1 << CS02);
        break;
    case 1024:
        TCCR0B |= (1 << CS02) | (1 << CS00);
        break;
    }
}

void avr8_timer_0_set_duty(const avr8_timer_0_config_t *cfg, uint8_t duty)
{
    if (cfg->output == PWM0_OC0A)
        OCR0A = duty;
    else
        OCR0B = duty;
}

// uint32_t pwm0_calc_frequency(const avr8_timer_pwm_0_config_t *cfg)
// {
//     uint32_t top = (cfg->mode == PWM0_PHASE_CORRECT || cfg->mode == PWM0_PHASE_CORRECT_OCR) ? 255 : 255;
//     return F_CPU / (cfg->prescaler * (top + 1));
// }

/*===================== TIMER1 =====================*/
void avr8_timer_1_init(const avr8_timer_1_config_t *cfg)
{
    // Set output pin
    switch (cfg->output)
    {
    case PWM1_OC1A:
        DDRB |= (1 << PB1);
        break;
    case PWM1_OC1B:
        DDRB |= (1 << PB2);
        break;
    }

    TCCR1A = 0;
    TCCR1B = 0;

    // Waveform setup (example for clarity; user must provide top if OCR/ICR modes)
    switch (cfg->mode)
    {
    case PWM1_NORMAL:
        break;
    case PWM1_PHASE_CORRECT_8BIT:
        TCCR1A |= (1 << WGM10);
        break;
    case PWM1_PHASE_CORRECT_9BIT:
        TCCR1A |= (1 << WGM11);
        break;
    case PWM1_PHASE_CORRECT_10BIT:
        TCCR1A |= (1 << WGM11) | (1 << WGM10);
        break;
    case PWM1_CTC_OCR1A:
        TCCR1B |= (1 << WGM12);
        break;
    case PWM1_FAST_8BIT:
        TCCR1A |= (1 << WGM10);
        TCCR1B |= (1 << WGM12);
        break;
    case PWM1_FAST_9BIT:
        TCCR1A |= (1 << WGM11);
        TCCR1B |= (1 << WGM12);
        break;
    case PWM1_FAST_10BIT:
        TCCR1A |= (1 << WGM11) | (1 << WGM10);
        TCCR1B |= (1 << WGM12);
        break;
    case PWM1_PHASE_FREQ_CORRECT_ICR1:
        TCCR1B |= (1 << WGM13);
        break;
    case PWM1_PHASE_FREQ_CORRECT_OCR1A:
        TCCR1A |= (1 << WGM10);
        TCCR1B |= (1 << WGM13);
        break;
    case PWM1_PHASE_CORRECT_ICR1:
        TCCR1B |= (1 << WGM13);
        break;
    case PWM1_PHASE_CORRECT_OCR1A:
        TCCR1A |= (1 << WGM10);
        TCCR1B |= (1 << WGM13);
        break;
    case PWM1_CTC_ICR1:
        TCCR1B |= (1 << WGM12) | (1 << WGM13);
        break;
    case PWM1_FAST_ICR1:
        TCCR1A |= (1 << WGM11);
        TCCR1B |= (1 << WGM13) | (1 << WGM12);
        break;
    case PWM1_FAST_OCR1A:
        TCCR1A |= (1 << WGM10) | (1 << WGM11);
        TCCR1B |= (1 << WGM12) | (1 << WGM13);
        break;
    }

    // Set TOP automatically based on mode
    switch (cfg->mode)
    {
    case PWM1_PHASE_CORRECT_8BIT:
    case PWM1_FAST_8BIT:
        ICR1 = 0x00FF;
        break;

    case PWM1_PHASE_CORRECT_9BIT:
    case PWM1_FAST_9BIT:
        ICR1 = 0x01FF;
        break;

    case PWM1_PHASE_CORRECT_10BIT:
    case PWM1_FAST_10BIT:
        ICR1 = 0x03FF;
        break;

    case PWM1_PHASE_FREQ_CORRECT_ICR1:
    case PWM1_PHASE_CORRECT_ICR1:
    case PWM1_FAST_ICR1:
        // Use ICR1 as TOP → leave it as-is or initialize to max
        ICR1 = 0xFFFF;
        break;

    case PWM1_PHASE_FREQ_CORRECT_OCR1A:
    case PWM1_PHASE_CORRECT_OCR1A:
    case PWM1_FAST_OCR1A:
        // Use OCR1A as TOP → initialize OCR1A if needed
        OCR1A = 0xFFFF;
        break;

    default:
        break;
    }

    // Output compare inverting mode
    if (cfg->inverting)
    {
        if (cfg->output == PWM1_OC1A)
            TCCR1A |= (1 << COM1A1) | (1 << COM1A0);
        else
            TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
    }
    else
    {
        if (cfg->output == PWM1_OC1A)
            TCCR1A |= (1 << COM1A1);
        else
            TCCR1A |= (1 << COM1B1);
    }

    avr8_timer_1_set_duty(cfg, cfg->duty);

    // Prescaler
    switch (cfg->prescaler)
    {
    case 1:
        TCCR1B |= (1 << CS10);
        break;
    case 8:
        TCCR1B |= (1 << CS11);
        break;
    case 64:
        TCCR1B |= (1 << CS11) | (1 << CS10);
        break;
    case 256:
        TCCR1B |= (1 << CS12);
        break;
    case 1024:
        TCCR1B |= (1 << CS12) | (1 << CS10);
        break;
    }
}

void avr8_timer_1_set_duty(const avr8_timer_1_config_t *cfg, uint16_t duty)
{
    // OCR1A-as-TOP modes: OC1A output not available
    bool ocr1a_as_top = (cfg->mode == PWM1_FAST_OCR1A ||
                         cfg->mode == PWM1_PHASE_CORRECT_OCR1A ||
                         cfg->mode == PWM1_PHASE_FREQ_CORRECT_OCR1A);

    if (cfg->output == PWM1_OC1A)
    {
        if (ocr1a_as_top)
        {
            // OC1A pin is disconnected → cannot set duty
            return; // or optionally assert/error
        }
        else
        {
            OCR1A = duty; // normal duty cycle
        }
    }
    else
    {
        OCR1B = duty; // duty for OC1B works in all modes
    }
}

/*===================== TIMER2 =====================*/
void avr8_timer_2_init(const avr8_timer_2_config_t *cfg)
{
    switch (cfg->output)
    {
    case PWM2_OC2A:
        DDRB |= (1 << PB3);
        break;
    case PWM2_OC2B:
        DDRD |= (1 << PD3);
        break;
    }

    TCCR2A = 0;
    TCCR2B = 0;

    // Waveform generation
    switch (cfg->mode)
    {
    case PWM2_NORMAL:
        break;
    case PWM2_PHASE_CORRECT:
        TCCR2A |= (1 << WGM20);
        break;
    case PWM2_CTC:
        TCCR2A |= (1 << WGM21);
        break;
    case PWM2_FAST:
        TCCR2A |= (1 << WGM21) | (1 << WGM20);
        break;
    case PWM2_PHASE_CORRECT_OCR:
        TCCR2A |= (1 << WGM20);
        TCCR2B |= (1 << WGM22);
        break;
    case PWM2_FAST_OCR:
        TCCR2A |= (1 << WGM21) | (1 << WGM20);
        TCCR2B |= (1 << WGM22);
        break;
    }

    if (cfg->inverting)
    {
        if (cfg->output == PWM2_OC2A)
            TCCR2A |= (1 << COM2A1) | (1 << COM2A0); // Inverting
        else
            TCCR2A |= (1 << COM2B1) | (1 << COM2B0); // Inverting
    }
    else
    {
        if (cfg->output == PWM2_OC2A)
            TCCR2A |= (1 << COM2A1);
        else
            TCCR2A |= (1 << COM2B1);
    }

    avr8_timer_2_set_duty(cfg, cfg->duty);

    // Prescaler
    switch (cfg->prescaler)
    {
    case 1:
        TCCR2B |= (1 << CS20);
        break;
    case 8:
        TCCR2B |= (1 << CS21);
        break;
    case 32:
        TCCR2B |= (1 << CS21) | (1 << CS20);
        break;
    case 64:
        TCCR2B |= (1 << CS22);
        break;
    case 128:
        TCCR2B |= (1 << CS22) | (1 << CS20);
        break;
    case 256:
        TCCR2B |= (1 << CS22) | (1 << CS21);
        break;
    case 1024:
        TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
        break;
    }
}

void avr8_timer_2_set_duty(const avr8_timer_2_config_t *cfg, uint8_t duty)
{
    if (cfg->output == PWM2_OC2A)
        OCR2A = duty;
    else
        OCR2B = duty;
}