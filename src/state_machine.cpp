#include "state_machine.h"
#include "syringe_pump.h"
#include "tmc2209_motor.h"

/* Private types ------------------------------- */

extern LiquidCrystal lcd;

uint32_t pumpSpeed;
uint32_t timeOnePulse;
uint32_t startTime;
uint32_t elapsedTime;

state_t currentState, nextState;
event_t externalEvent = NO_EVENT;

typedef void (*stateFunction_t)(void);

struct sStateAction
{
    stateFunction_t entryAction;
    stateFunction_t doAction;
    stateFunction_t exitAction;
};

struct sStateAction stateEntry[];

void stateCreate(state_t stateName,
                 stateFunction_t entryFunction,
                 stateFunction_t doFunction,
                 stateFunction_t exitFunction)
{
    stateEntry[stateName] = {entryFunction, doFunction, exitFunction};
}

/* Private function prototypes ------------------*/

void increaseFlowRate(void)
{
    if (flowrate < 20)
        flowrate++;
    else
    {
        lcd.clear();
        lcd.print("Warning: Flow rate reaches max limit");
        delay(3000);
    }
}

void decreaseFlowRate(void)
{
    if (flowrate > 1)
        flowrate--;
    else
    {
        lcd.clear();
        lcd.print("Warning: Flow rate reaches min limit");
        delay(3000);
    }
}

void displayFlowRate(void)
{
    lcd.clear();
    lcd.printf("Flow rate: %d", flowrate);
}

void displayWarning(void)
{
    lcd.clear();
    lcd.print("Warning: Pause the motor to set flow rate");
    delay(3000);
}

void startPumping(void)
{
    pumpSpeed = SP_calculateSpeed(flowrate);
    timeOnePulse = SP_timeOnePulse_us(pumpSpeed);
    motor_run(timeOnePulse);
    startTime = millis();
}

void displayTimeElapse(void)
{
    // Calculate the elapsed time
    elapsedTime = millis() - startTime; // Print the elapsed time every second
    if (elapsedTime % 1000 == 0)
    {
        lcd.clear();
        lcd.printf("Elapsed time: %d seconds", elapsedTime / 1000);
    }
}

event_t checkExternalEvent(void);
void runStateMachine(event_t event);

/* Private functions --------- ------------------*/

/**
 * @brief   Return event if a condition is met.
 * @param   None
 * @return  Event being triggered.
 *          NO_EVENT if no condition is met
 */
event_t checkExternalEvent(void)
{
    if (digitalRead(BTN_DEC) == LOW)
    {
        delay(20); // button debouncing
        while (digitalRead(BTN_DEC) == LOW)
            ; // wait until button released
        return E_BTN_DEC;
    }
    if (digitalRead(BTN_INC) == LOW)
    {
        delay(20); // button debouncing
        while (digitalRead(BTN_INC) == LOW)
            ; // wait until button released
        return E_BTN_INC;
    }
    if (digitalRead(BTN_START_STOP) == LOW)
    {
        delay(20); // button debouncing
        while (digitalRead(BTN_START_STOP) == LOW)
            ; // wait until button released
        return E_BTN_START_STOP;
    }
    //else
    return NO_EVENT;
}

uint8_t checkStateTransition(state_t currentState, state_t *nextState)
{
    event_t externalEvent = checkExternalEvent();

    switch (currentState) // check the current state
    {
    case S_IDLE:
        // check for any new event exist
        switch (externalEvent)
        {
        case E_BTN_DEC:
            *nextState = S_DEC;
            return 1;
        case E_BTN_INC:
            *nextState = S_INC;
            return 1;
        }
        break;
    case S_RUN:
        // check for any new event exist
        if (externalEvent == E_BTN_DEC || E_BTN_INC)
            displayWarning();
        break;
    }

    return 0;
}

/**
 * @brief   Implement change if a new event has occured.
 *          If a transition occurs, perform exitAction() of the current state,
 *          entryAction() of the next state and update the current state.
 *          Perform doAction() according to the current state no matter if
 *          a transition has occured or not.
 * @param   event Event returned from checkExternalEvent() function.
 * @return  None
 */
void runStateMachine(event_t event)
{
    static state_t currentState, nextState;

    if (checkStateTransition(currentState, &nextState)) // transistion has occurred
    {
        stateEntry[currentState].exitAction;
        stateEntry[nextState].entryAction;
        currentState = nextState; // update the state
    }
    stateEntry[currentState].doAction; // run main action of the current state
}
