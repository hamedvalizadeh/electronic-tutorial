#include "servo_linear.h"

uint16_t servo_angle_to_us(const servo_cfg_t *cfg, uint8_t angle)
{
    float step = (cfg->us_max - cfg->us_min) / 180.0f;
    return (uint16_t)(cfg->us_min + step * angle);
}

uint16_t servo_us_to_ticks(const servo_cfg_t *cfg, uint16_t pulse_us)
{
    float effective_tick = cfg->tick_us * cfg->mode_factor;
    return (uint16_t)(pulse_us / effective_tick);
}

uint16_t servo_get_rotation_wait_time_ms(const servo_cfg_t *cfg)
{
    int16_t delta = (int16_t)cfg->current_angle - (int16_t)cfg->last_angle;
    if (delta < 0)
    {
        delta = -delta;
    }
    // HINT: correct formulation is as follow, but to round it to ceiling we changed the formula
    // return (delta * cfg->speed_time_ms) / cfg->speed_deg;
    return (delta * cfg->speed_time_ms + (cfg->speed_deg / 2)) / cfg->speed_deg;
}

uint16_t servo_angle_pulse_ticks(servo_cfg_t *cfg, uint8_t angle)
{
    cfg->last_angle = cfg->current_angle;
    cfg->current_angle = angle;
    uint16_t us = servo_angle_to_us(cfg, angle);
    return servo_us_to_ticks(cfg, us);
}