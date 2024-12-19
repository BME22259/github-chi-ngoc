#include "motor.h"
#include "state_machine.h"
#include "syringe.h"
#include "main.h"

#define DEFAULT_FLOWRATE 10000 // default: 10 cc/min

/* Configure LCD */
#define LCD_RS 22
#define LCD_EN 23
#define LCD_D4 5
#define LCD_D5 18
#define LCD_D6 19
#define LCD_D7 21

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/* Configure syringe pump parameters */
syringe_handle_t syringe_pump_cfg =
    {
        .step_per_rev = 200,
        .microstepping = 16,
        .screw_pitch_mm = 2,
        .syringe_diameter_mm = 19.55};

/* Global variables */
float pumpSpeed, timeOneRev;
uint32_t timeOnePulse;
uint32_t startTime;
uint32_t elapsedTime;
uint32_t flowrate = DEFAULT_FLOWRATE;

/* Function prototypes */
// Set up functions
void initGPIO(void);
void initStateMachine(void);
// IDLE action
void displayFlowRate(void);
// INC action
void increaseFlowrate(void);
// DEC action
void decreaseFlowrate(void);
// RUN actions
void startPumping(void);
void displayTimeElapse(void);
void stopPumping(void);

/* Function definitions */
// Set up functions
void initGPIO()
{
  pinMode(BTN_DEC, INPUT);
  pinMode(BTN_INC, INPUT);
  pinMode(BTN_START_STOP, INPUT);
}

void initStateMachine()
{
  // Create states with their entry, do and exit actions
  SM_createState(S_IDLE, displayFlowRate, NULL, NULL);
  SM_createState(S_INC, increaseFlowrate, NULL, NULL);
  SM_createState(S_DEC, decreaseFlowrate, NULL, NULL);
  SM_createState(S_RUN, startPumping, displayTimeElapse, stopPumping);
  
  // Start the machine with IDLE entry action
  state[S_IDLE].entryAction();
}

// IDLE entry action
void displayFlowRate(void)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("Flow rate: ");
  lcd.setCursor(0, 1);
  lcd.printf("%d cc/min", flowrate); // print the current flow rate
}

// INC entry action
void increaseFlowrate(void)
{
  if (flowrate < 20)
  {
    flowrate++;
  }
  // If the flow rate reaches maximum limit
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

// DEC entry action
void decreaseFlowrate(void)
{
  if (flowrate > 1)
  {
    flowrate--;
  }
  // If the flow rate reaches minimum limit
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

// RUN entry actions
void startPumping(void)
{
  // Calculate syringe speed, time one revolution and time one pulse
  pumpSpeed = syringe_calculateSpeed(&syringe_pump_cfg, flowrate);
  Serial.print("Speed in mm/min: ");
  Serial.println(pumpSpeed);
  timeOneRev = syringe_timeOneRev_s(&syringe_pump_cfg, pumpSpeed);
  Serial.print("s per revolution: ");
  Serial.println(timeOneRev);
  timeOnePulse = syringe_timeOnePulse_us(&syringe_pump_cfg, timeOneRev);
  Serial.print("us per pulse: ");
  Serial.println(timeOnePulse);

  // Start the motor
  motor_start(timeOnePulse);

  // Keep track of the start time
  startTime = millis();
}

// RUN do actions
void displayTimeElapse(void)
{
  // Calculate the elapsed time
  elapsedTime = millis() - startTime; 

  // Print the elapsed time every second
  if (elapsedTime % 1000 == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("Elapsed time: ");
    lcd.setCursor(0, 1);
    lcd.printf("%d h %d m %d s", elapsedTime / (60 * 60 * 1000), (elapsedTime / (60 * 1000)) % 60, (elapsedTime / 1000) % 60);
  }
}

// RUN exit actions
void stopPumping(void)
{
  // Stop motor
  motor_stop();
  lcd.clear();
  lcd.printf("Stop motor");
  delay(2000);
  lcd.clear();
}

/* Main program */
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