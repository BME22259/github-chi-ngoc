#include "tmc2209_motor.h"
// define a null pointer to timer structure
static hw_timer_t *timer1 = NULL;
// extern a semaphore
extern SemaphoreHandle_t motorPauseSemaphore;

void IRAM_ATTR pulseOnTimer1()
{
    digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
}

void motor_init(void)
{
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

  timer1 = timerBegin(1, 80, true); // 1: timer 1, 80: prescaler, true: count up
  if (timer1 == NULL)
    printf("Error: timer fails");
  else
  {
    timerStop(timer1);
    timerAttachInterrupt(timer1, &pulseOnTimer1, true);
  }
}

void motor_run(uint32_t timeOnePulse_us)
{
  timerAlarmWrite(timer1, timeOnePulse_us, true);
  timerAlarmEnable(timer1);
  timerRestart(timer1);
  timerStart(timer1);
}
