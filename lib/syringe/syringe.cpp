#include "syringe.h"

float syringe_calculateSpeed(syringe_handle_t *SP_handle, uint32_t flowRate_mm3_min)
{
    float area_mm2, radius, speed_mm_min;
    radius = (SP_handle->syringe_diameter_mm) / 2;
    area_mm2 = 3.14 * radius * radius;
    speed_mm_min = (float)flowRate_mm3_min / area_mm2;
    return speed_mm_min;
}

float syringe_timeOneRev_s(syringe_handle_t *SP_handle, float speed_mm_min)
{
    if (SP_handle->gear_ratio == 0)
        SP_handle->gear_ratio = 1; 
    return (float)(SP_handle->screw_pitch_mm * 60) / (SP_handle->gear_ratio * speed_mm_min);
}

uint32_t syringe_timeOnePulse_us(syringe_handle_t *SP_handle, float timeOneRev_s)
{
    uint32_t timeOneMicrostep_us;
    timeOneMicrostep_us = timeOneRev_s * 1000000 / (SP_handle->microstepping * SP_handle->step_per_rev);
    return timeOneMicrostep_us / 2;
}