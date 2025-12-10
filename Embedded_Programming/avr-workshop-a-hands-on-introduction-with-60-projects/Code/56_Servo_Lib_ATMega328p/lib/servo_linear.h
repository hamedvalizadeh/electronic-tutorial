#ifndef SERVO_LINEAR_H
#define SERVO_LINEAR_H

#include <stdint.h>

/* A minimal servo configuration */
typedef struct
{
    uint16_t us_min;
    uint16_t us_max;
    float tick_us;
    uint8_t mode_factor;
    uint8_t speed_deg;
    uint16_t speed_time_ms;
    uint8_t current_angle;
    uint8_t last_angle;
} servo_cfg_t;

uint16_t servo_angle_pulse_ticks(servo_cfg_t *cfg, uint8_t angle);
uint16_t servo_get_rotation_wait_time_ms(const servo_cfg_t *cfg);

#endif
