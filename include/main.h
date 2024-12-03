#ifndef MAIN_H__
#define MAIN_H__

#include <Arduino.h>
#include <LiquidCrystal.h>

#define BTN_INC 32        // to increase flow rate or syring size
#define BTN_DEC 35       // to decrease flow rate or syring size
#define BTN_START_STOP 34 // to start or stop pumping

extern LiquidCrystal lcd;

#endif //MAIN_H__