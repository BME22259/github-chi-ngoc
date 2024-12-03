#include "tmc2209_motor.h"
#include "syringe_pump.h"
#include "state_machine.h"
#include "LiquidCrystal.h"
#include "Arduino.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  LCD_init();
  
  GPIO_init();
  
  motor_init();
}
void loop() {
  // nothing here
}
