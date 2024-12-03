#ifndef MOTOR_H__
#define MOTOR_H__
#include <Arduino.h>

#define STEP_PIN 2 
#define EN_PIN 4
#define DIR_PIN 15

void motor_init(void);
void motor_start(uint32_t);
void motor_stop(void);

#endif // MOTOR_H__