#include <Arduino.h>
#include "unity.h"

// Example for a 1.8-degree motor with 1/16 microstepping, 2mm pitch and no gear box attached to .
#define STEP_PER_REV 200
#define MICROSTEPPING 16
#define MICROSTEP_PER_REV (STEP_PER_REV * MICROSTEPPING)
#define SCREW_PITCH_MM 2
#define GEARBOX 1
#define SYRINGE_DIAMETER_MM 14.55

typedef struct
{
    uint16_t step_per_rev;     // stepper motor parameter, can be 200, 400 ...
    uint8_t microstepping;     // driver mode, can be 2, 4, 8, 16, 32...
    uint8_t screw_pitch_mm;    // pitch of the lead screw, can be 2mm, 5mm ...
    uint8_t gear_ratio;        // gear ratio
    float syringe_diameter_mm; // syringe diameter
} SP_handle_t;

#define BTN_INC 7        // to increase flow rate or syring size
#define BTN_DEC 8        // to decrease flow rate or syring size
#define BTN_START_STOP 9 // to start or stop pumping

float s, rev;

float SP_calculateSpeed(SP_handle_t *SP_handle, uint32_t flowRate_mm3_min)
{
    float area_mm2, radius, speed_mm_min;
    radius = (SP_handle->syringe_diameter_mm) / 2;
    area_mm2 = 3.14 * radius * radius;
    speed_mm_min = (float)flowRate_mm3_min / area_mm2;
    return speed_mm_min;
}

float SP_timeOneRev_s(SP_handle_t *SP_handle, float speed_mm_min)
{
    if (SP_handle->gear_ratio == 0)
        SP_handle->gear_ratio = 1; // by default, the gear ratio is 1
    return (float)(SP_handle->screw_pitch_mm * 60) / (SP_handle->gear_ratio * speed_mm_min);
}

uint32_t SP_timeOnePulse_us(SP_handle_t *SP_handle, float timeOneRev_s)
{
    uint32_t timeOneMicrostep_us;
    timeOneMicrostep_us = timeOneRev_s * 1000000 / (SP_handle->microstepping * SP_handle->step_per_rev);
    return timeOneMicrostep_us / 2;
}

SP_handle_t handle =
    {
        .step_per_rev = 200,
        .microstepping = 16,
        .screw_pitch_mm = 2,
        .syringe_diameter_mm = 14.55};

void test_speed(void)
{
    s = SP_calculateSpeed(&handle, 3000);
    TEST_ASSERT_EQUAL(18, s);
}

void test_revolution(void)
{
    rev = SP_timeOneRev_s(&handle, s);
    TEST_ASSERT_EQUAL(66, rev * 10);
}

void test_micro(void)
{
    uint32_t result = SP_timeOnePulse_us(&handle, rev);
    TEST_ASSERT_EQUAL(1041, result);
}

void setup()
{
    UNITY_BEGIN();

    RUN_TEST(test_speed);
    RUN_TEST(test_revolution);
    RUN_TEST(test_micro);

    UNITY_END();
}

void loop()
{
}