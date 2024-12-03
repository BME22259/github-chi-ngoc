#include "syringe.h"

float SP_calculateSpeed(float flowRate_cm3_min)
{
    float area_mm2, radius, speed_mm_min;
    radius = SYRINGE_DIAMETER_MM / 2;
    area_mm2 = 3.14 * radius * radius;
    speed_mm_min = flowRate_cm3_min * 1000 / area_mm2;
    return speed_mm_min;
}

float SP_timeOneRev_s(float speed_mm_min)
{
    return (float)(SCREW_PITCH_MM * 60) / (GEARBOX * speed_mm_min) ;
}

uint32_t SP_timeOnePulse_us(float timeOneRev_s)
{
    uint32_t timeOneMicrostep_us;
    timeOneMicrostep_us = timeOneRev_s * 1000000 / MICROSTEP_PER_REV;
    return timeOneMicrostep_us / 2;
}