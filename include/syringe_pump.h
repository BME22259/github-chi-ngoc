#ifndef SYRINGEPUMP_H__
#define SYRINGEPUMP_H__

#include <Arduino.h>
#include <LiquidCrystal.h>

// Example for a 1.8-degree motor with 1/16 microstepping, 2mm pitch and no gear box.
#define STEP_PER_REV 		200
#define MICROSTEPPING 	16
#define MICROSTEP_PER_REV 	(STEP_PER_REV * MICROSTEPPING) 
#define SCREW_PITCH_MM 		2
#define GEARBOX 			1
#define SYRINGE_DIAMETER_MM 	19.55

#define BTN_INC 7        // to increase flow rate or syring size
#define BTN_DEC 8        // to decrease flow rate or syring size
#define BTN_START_STOP 9 // to start or stop pumping

#define LCD_RS      22
#define LCD_EN      23
#define LCD_D4      5
#define LCD_D5      18
#define LCD_D6      19
#define LCD_D7      21

uint16_t flowrate = 10; // default: 10 cc/min

void LCD_init(void);
void GPIO_init(void);

float SP_calculateSpeed(float flowRate_mm3_min);
uint32_t SP_timeOnePulse_us(float speed_mm_min);

#endif //SYRINGEPUMP_H__