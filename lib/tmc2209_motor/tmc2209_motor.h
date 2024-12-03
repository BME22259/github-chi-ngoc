#ifndef TMC_MOTOR_H__
#define TMC_MOTOR_H__
#include <Arduino.h>

#define STEP_PIN 32
#define DIR_PIN 33

void motor_init(void);
void motor_run(uint32_t);

#endif