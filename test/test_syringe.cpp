#include <Arduino.h>
#include "unity.h"

// Example for a 1.8-degree motor with 1/16 microstepping, 2mm pitch and no gear box.
#define STEP_PER_REV 200
#define MICROSTEPPING 16
#define MICROSTEP_PER_REV (STEP_PER_REV * MICROSTEPPING)
#define SCREW_PITCH_MM 2
#define GEARBOX 1
#define SYRINGE_DIAMETER_MM 14.55

#define BTN_INC 7        // to increase flow rate or syring size
#define BTN_DEC 8        // to decrease flow rate or syring size
#define BTN_START_STOP 9 // to start or stop pumping

float s, rev;

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

void test_speed(void)
{
    s = SP_calculateSpeed(3);
    TEST_ASSERT_EQUAL(18, s);
}

void test_revolution(void)
{
    rev = SP_timeOneRev_s(s);
    TEST_ASSERT_EQUAL(66, rev*10);
}

void test_micro(void)
{
    uint32_t result = SP_timeOnePulse_us(rev);
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