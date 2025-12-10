#include "avr8_timer.h"
#include <util/atomic.h>

/*===================== TIMER0 =====================*/

uint8_t avr8_timer_0_top = 0;

void avr8_timer_0_set_compare_mode(const avr8_timer_0_config_t *cfg)
{
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
}

void avr8_timer_0_set_top(const avr8_timer_0_config_t *cfg)
{
    avr8_timer_0_top = cfg->top_variable;

    switch (cfg->mode)
    {
    case PWM0_NORMAL:
    case PWM0_PHASE_CORRECT:
    case PWM0_FAST:
        avr8_timer_0_top = 0xFF;
        break;

    case PWM0_CTC:
    case PWM0_PHASE_CORRECT_OCR:
    case PWM0_FAST_OCR:
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { OCR0A = avr8_timer_0_top; }
        break;

    default:
        break;
    }
}

void avr8_timer_0_set_compare_inverting(const avr8_timer_0_config_t *cfg)
{
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
}

void avr8_timer_0_set_prescaler(const avr8_timer_0_config_t *cfg)
{
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

void avr8_timer_0_init(const avr8_timer_0_config_t *cfg)
{
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

    avr8_timer_0_set_compare_mode(cfg);
    avr8_timer_0_set_top(cfg);
    avr8_timer_0_set_compare_inverting(cfg);
    avr8_timer_0_set_duty(cfg, cfg->duty_percent);
    avr8_timer_0_set_prescaler(cfg);
}

// void avr8_timer_0_set_pulse_ticks(const avr8_timer_0_config_t *cfg, uint8_t pulse_ticks)
// {
//     if (cfg->output == PWM0_OC0A)
//         OCR0A = pulse_ticks;
//     else
//         OCR0B = pulse_ticks;
// }

void avr8_timer_0_set_pulse_ticks(const avr8_timer_0_config_t *cfg, uint16_t pulse_ticks)
{
    // clamp to hardware limit (8-bit)
    if (pulse_ticks > 255)
        pulse_ticks = 255;

    if (cfg->output == PWM0_OC0A)
        OCR0A = (uint8_t)pulse_ticks;
    else
        OCR0B = (uint8_t)pulse_ticks;
}

void avr8_timer_0_set_duty(const avr8_timer_0_config_t *cfg, uint8_t duty)
{
    if (duty > 100)
    {
        duty = 100;
    }

    uint8_t pulse_ticks = avr8_timer_0_top * duty / 100;

    avr8_timer_0_set_pulse_ticks(cfg, pulse_ticks);
}

uint8_t avr8_timer_0_get_top()
{
    return avr8_timer_0_top;
}

float avr8_timer_0_get_tick_us(const avr8_timer_0_config_t *cfg)
{
    float tick = ((float)cfg->prescaler / (float)F_CPU) * 1000000.0f;
    return tick;
}

uint8_t avr8_timer_0_get_mode_factor(const avr8_timer_0_config_t *cfg)
{
    switch (cfg->mode)
    {
    case PWM0_PHASE_CORRECT:
    case PWM0_PHASE_CORRECT_OCR:
        return 2;
    case PWM0_FAST:
    case PWM0_FAST_OCR:
    case PWM0_CTC:
    case PWM0_NORMAL:
    default:
        return 1;
    }
}

/*===================== TIMER1 =====================*/

uint16_t avr8_timer_1_top = 0;

void avr8_timer_1_set_compare_mode(const avr8_timer_1_config_t *cfg)
{
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
}

void avr8_timer_1_set_top(const avr8_timer_1_config_t *cfg)
{
    avr8_timer_1_top = cfg->top_variable;

    switch (cfg->mode)
    {
    case PWM1_NORMAL:
        avr8_timer_1_top = 0xFFFF;
        break;

    case PWM1_PHASE_CORRECT_8BIT:
    case PWM1_FAST_8BIT:
        avr8_timer_1_top = 0xFF;
        break;

    case PWM1_PHASE_CORRECT_9BIT:
    case PWM1_FAST_9BIT:
        avr8_timer_1_top = 0x1FF;
        break;

    case PWM1_PHASE_CORRECT_10BIT:
    case PWM1_FAST_10BIT:
        avr8_timer_1_top = 0x3FF;
        break;

    case PWM1_PHASE_FREQ_CORRECT_ICR1:
    case PWM1_PHASE_CORRECT_ICR1:
    case PWM1_FAST_ICR1:
    case PWM1_CTC_ICR1:
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { ICR1 = avr8_timer_1_top; }
        break;

    case PWM1_PHASE_FREQ_CORRECT_OCR1A:
    case PWM1_PHASE_CORRECT_OCR1A:
    case PWM1_FAST_OCR1A:
    case PWM1_CTC_OCR1A:
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { OCR1A = avr8_timer_1_top; }
        break;

    default:
        break;
    }
}

void avr8_timer_1_set_compare_inverting(const avr8_timer_1_config_t *cfg)
{
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
}

void avr8_timer_1_set_prescaler(const avr8_timer_1_config_t *cfg)
{
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

bool avr8_timer_1_is_ocr1a_top(const avr8_timer_1_config_t *cfg)
{
    // OCR1A-as-TOP modes: OC1A output not available
    return (cfg->mode == PWM1_FAST_OCR1A ||
            cfg->mode == PWM1_PHASE_CORRECT_OCR1A ||
            cfg->mode == PWM1_PHASE_FREQ_CORRECT_OCR1A);
}

void avr8_timer_1_set_pulse_ticks(const avr8_timer_1_config_t *cfg, uint16_t pulse_ticks)
{
    if (cfg->output == PWM1_OC1A)
    {
        if (avr8_timer_1_is_ocr1a_top(cfg))
        {
            // OC1A unavailable in this mode
            return; // or assert/error
        }
        else
        {
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
            {
                OCR1A = pulse_ticks;
            }
        }
    }
    else
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            OCR1B = pulse_ticks;
        }
    }
}

void avr8_timer_1_set_duty(const avr8_timer_1_config_t *cfg, uint8_t duty)
{
    if (duty > 100)
    {
        duty = 100;
    }

    uint16_t pulse_ticks = (uint32_t)avr8_timer_1_top * duty / 100;

    avr8_timer_1_set_pulse_ticks(cfg, pulse_ticks);
}

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

    avr8_timer_1_set_compare_mode(cfg);
    avr8_timer_1_set_top(cfg);
    avr8_timer_1_set_compare_inverting(cfg);
    avr8_timer_1_set_duty(cfg, cfg->duty_percent);
    avr8_timer_1_set_prescaler(cfg);
}

uint16_t avr8_timer_1_get_top()
{
    return avr8_timer_1_top;
}

float avr8_timer_1_get_tick_us(const avr8_timer_1_config_t *cfg)
{
    float tick = ((float)cfg->prescaler / (float)F_CPU) * 1000000.0f;
    return tick;
}

uint8_t avr8_timer_1_get_mode_factor(const avr8_timer_1_config_t *cfg)
{
    switch (cfg->mode)
    {
    /* Phase-correct 8/9/10-bit */
    case PWM1_PHASE_CORRECT_8BIT:
    case PWM1_PHASE_CORRECT_9BIT:
    case PWM1_PHASE_CORRECT_10BIT:
    /* Phase/Freq-correct and Phase-correct using ICR1 or OCR1A as TOP */
    case PWM1_PHASE_FREQ_CORRECT_ICR1:
    case PWM1_PHASE_FREQ_CORRECT_OCR1A:
    case PWM1_PHASE_CORRECT_ICR1:
    case PWM1_PHASE_CORRECT_OCR1A:
        return 2.0f;

    /* Fast PWM modes, CTC, normal: single-slope behaviour */
    case PWM1_NORMAL:
    case PWM1_CTC_OCR1A:
    case PWM1_FAST_8BIT:
    case PWM1_FAST_9BIT:
    case PWM1_FAST_10BIT:
    case PWM1_CTC_ICR1:
    case PWM1_FAST_ICR1:
    case PWM1_FAST_OCR1A:
    default:
        return 1.0f;
    }
}

/*===================== TIMER2 =====================*/

uint8_t avr8_timer_2_top = 0;

void avr8_timer_2_set_compare_mode(const avr8_timer_2_config_t *cfg)
{
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
}

void avr8_timer_2_set_top(const avr8_timer_2_config_t *cfg)
{
    avr8_timer_2_top = cfg->top_variable;

    switch (cfg->mode)
    {
    case PWM2_NORMAL:
    case PWM2_PHASE_CORRECT:
    case PWM2_FAST:
        avr8_timer_2_top = 0xFF;
        break;

    case PWM2_CTC:
    case PWM2_PHASE_CORRECT_OCR:
    case PWM2_FAST_OCR:
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { OCR2A = avr8_timer_2_top; }
        break;

    default:
        break;
    }
}

void avr8_timer_2_set_compare_inverting(const avr8_timer_2_config_t *cfg)
{
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
}

void avr8_timer_2_set_prescaler(const avr8_timer_2_config_t *cfg)
{
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

void avr8_timer_2_set_pulse_ticks(const avr8_timer_2_config_t *cfg, uint8_t duty)
{
    if (cfg->output == PWM2_OC2A)
        OCR2A = duty;
    else
        OCR2B = duty;
}

void avr8_timer_2_set_duty(const avr8_timer_2_config_t *cfg, uint8_t duty)
{
    if (duty > 100)
    {
        duty = 100;
    }

    uint8_t pulse_ticks = avr8_timer_2_top * duty / 100;

    avr8_timer_2_set_pulse_ticks(cfg, pulse_ticks);
}

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

    avr8_timer_2_set_compare_mode(cfg);
    avr8_timer_2_set_top(cfg);
    avr8_timer_2_set_compare_inverting(cfg);
    avr8_timer_2_set_duty(cfg, cfg->duty_percent);
    avr8_timer_2_set_prescaler(cfg);
}

uint8_t avr8_timer_2_get_top()
{
    return avr8_timer_2_top;
}

float avr8_timer_2_get_tick_us(const avr8_timer_2_config_t *cfg)
{
    float tick = ((float)cfg->prescaler / (float)F_CPU) * 1000000.0f;
    return tick;
}

uint8_t avr8_timer_2_get_mode_factor(const avr8_timer_2_config_t *cfg)
{
    switch (cfg->mode)
    {
    /* Phase-correct (up/down) */
    case PWM2_PHASE_CORRECT:
    case PWM2_PHASE_CORRECT_OCR: // TOP = OCR2A, still up/down
        return 2.0f;

    /* Fast, CTC, normal: single-slope behaviour */
    case PWM2_NORMAL:
    case PWM2_CTC:
    case PWM2_FAST:
    case PWM2_FAST_OCR: // TOP = OCR2A but single-slope
    default:
        return 1.0f;
    }
}