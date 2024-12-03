#include "unity.h"
#include <Arduino.h>

#define STEP_PIN 2

static hw_timer_t *timer1 = NULL;

void IRAM_ATTR pulseOnTimer1()
{
    digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
}

BaseType_t motor_init(void)
{
    pinMode(STEP_PIN, OUTPUT);

    timer1 = timerBegin(1, 80, true); // 1: timer 1, 80: prescaler, true: count up
    if (timer1 == NULL)
    {
        printf("Error: timer fails");
        return 1;
    }
    timerAttachInterrupt(timer1, &pulseOnTimer1, true);
    timerAlarmWrite(timer1, 1000000, true);
    timerAlarmEnable(timer1);
    timerStart(timer1);
    printf("timer success");

    return pdPASS;
}

void test_timer(void)
{
    BaseType_t result = motor_init();
    TEST_ASSERT_EQUAL(pdPASS, result);
}

void setup()
{
    delay(2000); // service delay
    Serial.begin(115200);
    UNITY_BEGIN();
    RUN_TEST(test_timer);
    vTaskDelay(10000/portTICK_PERIOD_MS);
    UNITY_END();
}

void loop()
{

}
