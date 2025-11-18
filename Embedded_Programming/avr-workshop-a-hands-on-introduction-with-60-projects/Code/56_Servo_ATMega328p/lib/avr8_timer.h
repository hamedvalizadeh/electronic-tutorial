#ifndef AVR8_TIMER_PWM
#define AVR8_TIMER_PWM

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/*===================== TIMER0 (8-bit) =====================*/
typedef enum
{
    PWM0_NORMAL,
    PWM0_PHASE_CORRECT,
    PWM0_FAST,
    PWM0_CTC,               // TOP = OCR0A
    PWM0_PHASE_CORRECT_OCR, // TOP = OCR0A
    PWM0_FAST_OCR           // TOP = OCR0A
} avr8_timer_0_mode_t;

typedef enum
{
    PWM0_OC0A,
    PWM0_OC0B
} avr8_timer_0_output_t;

typedef struct
{
    avr8_timer_0_mode_t mode;
    uint16_t prescaler;
    avr8_timer_0_output_t output;
    uint8_t duty_percent;
    uint8_t top_variable;
    bool inverting;
} avr8_timer_0_config_t;

void avr8_timer_0_init(const avr8_timer_0_config_t *cfg);
void avr8_timer_0_set_duty(const avr8_timer_0_config_t *cfg, uint8_t duty);
void avr8_timer_0_set_duty_percent(const avr8_timer_0_config_t *cfg, uint8_t percent);
uint8_t avr8_timer_0_get_top();

/*===================== TIMER1 (16-bit) =====================*/
typedef enum
{
    PWM1_NORMAL,
    PWM1_PHASE_CORRECT_8BIT,
    PWM1_PHASE_CORRECT_9BIT,
    PWM1_PHASE_CORRECT_10BIT,
    PWM1_CTC_OCR1A, // TOP = OCR1A
    PWM1_FAST_8BIT,
    PWM1_FAST_9BIT,
    PWM1_FAST_10BIT,
    PWM1_PHASE_FREQ_CORRECT_ICR1,  // TOP = ICR1
    PWM1_PHASE_FREQ_CORRECT_OCR1A, // TOP = OCR1A
    PWM1_PHASE_CORRECT_ICR1,       // TOP = ICR1
    PWM1_PHASE_CORRECT_OCR1A,      // TOP = OCR1A
    PWM1_CTC_ICR1,                 // TOP = ICR1
    PWM1_FAST_ICR1,                // TOP = ICR1
    PWM1_FAST_OCR1A                // TOP = OCR1A
} avr8_timer_1_mode_t;

typedef enum
{
    PWM1_OC1A,
    PWM1_OC1B
} avr8_timer_1_output_t;

typedef struct
{
    avr8_timer_1_mode_t mode;
    uint16_t prescaler;
    avr8_timer_1_output_t output;
    uint8_t duty_percent;
    uint16_t top_variable;
    bool inverting;
} avr8_timer_1_config_t;

void avr8_timer_1_init(const avr8_timer_1_config_t *cfg);
void avr8_timer_1_set_duty(const avr8_timer_1_config_t *cfg, uint16_t duty);
void avr8_timer_1_set_duty_percent(const avr8_timer_1_config_t *cfg, uint8_t percent);
uint16_t avr8_timer_1_get_top();

/*===================== TIMER2 (8-bit) =====================*/
typedef enum
{
    PWM2_NORMAL,
    PWM2_PHASE_CORRECT,
    PWM2_CTC,
    PWM2_FAST,
    PWM2_PHASE_CORRECT_OCR, // TOP = OCR2A
    PWM2_FAST_OCR           // TOP = OCR2A
} avr8_timer_2_mode_t;

typedef enum
{
    PWM2_OC2A,
    PWM2_OC2B
} avr8_timer_2_output_t;

typedef struct
{
    avr8_timer_2_mode_t mode;
    uint16_t prescaler;
    avr8_timer_2_output_t output;
    uint8_t duty;
    bool inverting;
} avr8_timer_2_config_t;

void avr8_timer_2_init(const avr8_timer_2_config_t *cfg);
void avr8_timer_2_set_duty(const avr8_timer_2_config_t *cfg, uint8_t duty);

#endif // AVR8_TIMER_PWM