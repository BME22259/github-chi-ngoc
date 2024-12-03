#include "syringe_pump.h"

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void LCD_init(void)
{
    lcd.begin(16,2);
    lcd.print("Initializing...");
}

void GPIO_init(void)
{
    pinMode(BTN_DEC, INPUT);
    pinMode(BTN_INC, INPUT);
    pinMode(BTN_START_STOP, INPUT);
}

float SP_calculateSpeed(float flowRate_mm3_min){
  float area_mm2, radius, speed_mm_min;
  radius = SYRINGE_DIAMETER_MM/2;
  area_mm2 = 3.14*radius*radius;
  speed_mm_min = flowRate_mm3_min/area_mm2;
  return speed_mm_min; 
}

uint32_t SP_timeOnePulse_us(float speed_mm_min){
	uint32_t timeOneMicrostep_us;float timeOneRev_s;
	timeOneRev_s = SCREW_PITCH_MM/(GEARBOX*speed_mm_min)*60;
	timeOneMicrostep_us = timeOneRev_s*1000000/MICROSTEP_PER_REV;
	return timeOneMicrostep_us/2;
}

