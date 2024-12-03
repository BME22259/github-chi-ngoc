#include "motor.h"
// define a null pointer to timer structure
static hw_timer_t *timer1 = NULL;

/* Timer interrupt function ---------------------------------- */
void IRAM_ATTR pulseOnTimer1()
{
  digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
}

/**
 * @brief Set timer 1 as  the motor timer.
 *        With prescaler of 80, 1 timer tick is equal to 1us
 */
void timer1_init(void)
{
  timer1 = timerBegin(1, 80, true); // 1: timer 1, 80: prescaler, true: count up
  if (timer1 == NULL)
    Serial.println("Error: timer fails");
  else
  {
    timerAttachInterrupt(timer1, &pulseOnTimer1, true);
  }
}

/**
 * @brief Init motor GPIO and timer 1.
 */
void motor_init(void)
{
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  timer1_init();

  digitalWrite(EN_PIN, HIGH);
}

/**
 * @brief Start motor by enabling pulse generation by timer 1 and set driver's enable pin low
 * @param timeOnePulse_us time in microseconds to toggle the driver's step pin
 */
void motor_start(uint32_t timeOnePulse_us)
{
  timerAlarmWrite(timer1, timeOnePulse_us, true);
  timerAlarmEnable(timer1);
  timerRestart(timer1);
  timerStart(timer1);
  digitalWrite(EN_PIN, LOW);
}

/**
 * @brief Stop motor by disabling timer 1 and set driver's enable pin high
 */
void motor_stop(void)
{
  digitalWrite(EN_PIN, HIGH);
  timerStop(timer1);
}
