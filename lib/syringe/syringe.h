#ifndef SYRINGE_H__
#define SYRINGE_H__

#include <Arduino.h>

// Example for a 1.8-degree motor with 1/16 microstepping, 2mm pitch and no gear box.
#define STEP_PER_REV 		200
#define MICROSTEPPING 	16
#define MICROSTEP_PER_REV 	(STEP_PER_REV * MICROSTEPPING) 
#define SCREW_PITCH_MM 		2
#define GEARBOX 			1
#define SYRINGE_DIAMETER_MM 	19.55

float SP_calculateSpeed(float flowRate_mm3_min);
float SP_timeOneRev_s(float speed_mm_min);
uint32_t SP_timeOnePulse_us(float timeOneRev_s);

#endif //SYRINGE_H__