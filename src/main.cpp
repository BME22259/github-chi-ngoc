#include "motor.h"
#include "state_machine.h"
#include "syringe.h"
#include "main.h"

#define LCD_RS 22
#define LCD_EN 23
#define LCD_D4 5
#define LCD_D5 18
#define LCD_D6 19
#define LCD_D7 21
#define DEFAULT_FLOWRATE 10 // default: 10 cc/min

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

float pumpSpeed, timeOneRev;
uint32_t timeOnePulse;
uint32_t startTime;
uint32_t elapsedTime;

uint16_t flowrate = DEFAULT_FLOWRATE;

void initGPIO()
{
  pinMode(BTN_DEC, INPUT);
  pinMode(BTN_INC, INPUT);
  pinMode(BTN_START_STOP, INPUT);

  pinMode(2, OUTPUT);
}

void displayFlowRate(void)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("Flow rate: ");
  lcd.setCursor(0, 1);
  lcd.printf("%d cc/min", flowrate);
}

void displayTimeElapse(void)
{
  // Calculate the elapsed time
  elapsedTime = millis() - startTime; // Print the elapsed time every second
  if (elapsedTime % 1000 == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("Elapsed time: ");
    lcd.setCursor(0, 1);
    lcd.printf("%d h %d m %d s", elapsedTime / (60 * 60 * 1000), (elapsedTime / (60 * 1000)) % 60, (elapsedTime / 1000) % 60);
  }
}

void startPumping(void)
{
  pumpSpeed = SP_calculateSpeed(flowrate);
  Serial.print("Speed in mm/min: ");
  Serial.println(pumpSpeed);
  timeOneRev = SP_timeOneRev_s(pumpSpeed);
  Serial.print("s per revolution: ");
  Serial.println(timeOneRev);
  timeOnePulse = SP_timeOnePulse_us(timeOneRev);
  Serial.print("us per pulse: ");
  Serial.println(timeOnePulse);

  motor_start(timeOnePulse);
  startTime = millis();
}

void stopPumping(void)
{
  motor_stop();
  lcd.clear();
  lcd.printf("Stop motor");
  delay(2000);
  lcd.clear();
  Serial.print("Elapsed time in ms: ");
  Serial.println(elapsedTime);
}

void increaseFlowrate(void)
{
  if (flowrate < 20)
  {
    flowrate++;
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Warning: ");

    char msg[] = "reach maximum";
    lcd.setCursor(16 - strlen(msg), 1);
    lcd.print(msg);
    delay(2000);
    lcd.clear();
  }
}

void decreaseFlowrate(void)
{
  if (flowrate > 1)
  {
    flowrate--;
  }

  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Warning: ");
    char msg[] = "reach minimum";
    lcd.setCursor(16 - strlen(msg), 1);
    lcd.print(msg);
    delay(2000);
    lcd.clear();
  }
}

void initStateMachine()
{
  SM_createState(S_IDLE, displayFlowRate, NULL, NULL);
  SM_createState(S_RUN, startPumping, displayTimeElapse, stopPumping);
  SM_createState(S_INC, increaseFlowrate, NULL, NULL);
  SM_createState(S_DEC, decreaseFlowrate, NULL, NULL);

  stateEntry[S_IDLE].entryAction();
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  initGPIO();
  motor_init();

  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(3000);
  lcd.clear();

  initStateMachine();
}

void loop()
{
  SM_runMachine();
}
