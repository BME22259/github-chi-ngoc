#ifndef SYRINGE_H__
#define SYRINGE_H__

#include <Arduino.h>

typedef struct
{
    uint16_t step_per_rev;     // stepper motor parameter, can be 200, 400 ...
    uint8_t microstepping;     // driver mode, can be 2, 4, 8, 16, 32...
    uint8_t screw_pitch_mm;    // pitch of the lead screw, can be 2mm, 5mm ...
    uint8_t gear_ratio;        // gear ratio, by default is 1
    float syringe_diameter_mm; // syringe diameter in mm
} syringe_handle_t;

float syringe_calculateSpeed(syringe_handle_t *, uint32_t);
float syringe_timeOneRev_s(syringe_handle_t *, float);
uint32_t syringe_timeOnePulse_us(syringe_handle_t *, float);

#endif // SYRINGE_H__