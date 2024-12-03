/*
 * State Machine Template
 * Description of all states and what triggers each event
 * This code is part of Medical Design Course - BM017IU
 */

/* Include libraries---------------------------- */
#include "main.h"
#include "state_machine.h"

/* Extern variables ---------------------------- */
struct sStateAction stateEntry[NUM_STATES];

/* Private functions ---------------------------*/

/**
 * @brief   Return event if a condition is met.
 * @param   None
 * @return  Event being triggered.
 *          NO_EVENT if no condition is met
 */
event_t checkEvent(void)
{
    if (digitalRead(BTN_DEC) == LOW)
    {
        delay(50); // button debouncing
        while (digitalRead(BTN_DEC) == LOW)
            ; // wait until button released
        return E_BTN_DEC;
    }
    if (digitalRead(BTN_INC) == LOW)
    {
        delay(50); // button debouncing
        while (digitalRead(BTN_INC) == LOW)
            ; // wait until button released
        return E_BTN_INC;
    }
    if (digitalRead(BTN_START_STOP) == LOW)
    {
        delay(50); // button debouncing
        while (digitalRead(BTN_START_STOP) == LOW)
            ; // wait until button released
        return E_BTN_START_STOP;
    }
    // else
    return NO_EVENT;
}

/**
 * @brief   Rule for state transition - describe what happens to the machine when an event occurs.
 * @param   currentState current state's index
 * @param   nextState pointer to the next state's index
 * 
 * @return  1 if a new event has occured. 0 if there is no event.
 */
uint8_t checkStateTransition(state_t currentState, state_t *nextState)
{
    event_t event = checkEvent();

    switch (currentState) // check the current state
    {
    case S_IDLE:
        switch (event) // check for any new event exist
        {
        case E_BTN_DEC:
            *nextState = S_DEC; // set next state
            return 1;
        case E_BTN_INC:
            *nextState = S_INC; // set next state
            return 1;
        case E_BTN_START_STOP:
            *nextState = S_RUN; // set next state
            return 1;
        }
        break;
    case S_RUN:
        switch (event) // check for any new event exist
        {
        case E_BTN_DEC:
        case E_BTN_INC:
            // self-transition event, do not trigger transition but perform some action instead
            // display warning on the LCD screen
            lcd.clear();

            lcd.setCursor(0, 0);
            lcd.print("Warning: ");
            lcd.setCursor(0, 1);
            lcd.print("Stop motor first");

            delay(2000);
            lcd.clear();

            break;
        case E_BTN_START_STOP:
            *nextState = S_IDLE; // set next state
            return 1;
        }
        break;
    case S_INC:
    case S_DEC:
        *nextState = S_IDLE; // default next state for S_INC and S_DEC is S_IDLE
        return 1;
    }

    return 0; // if there is no change in state, return 0
}

/* Public functions ---------------------------*/

/**
 * @brief   create a state with its name and actions.
 *          Functions for entry, do and exit must accept no parameter and return nothing.
 * 
 * @param   stateName name of the state, defined in state_machine.h
 * @param   entryFunction name of the state's entry action.
 * @param   doFunction name of the state's do action.
 * @param   exitFunction name the state's exit action.\
 * 
 * @return  None
 */
void SM_createState(state_t stateName,
                    stateFunction_t entryFunction,
                    stateFunction_t doFunction,
                    stateFunction_t exitFunction)
{
    stateEntry[stateName] = {entryFunction, doFunction, exitFunction};
}

/**
 * @brief   Perform state actions
 *          If a transition occurs, perform exitAction() of the current state,
 *          entryAction() of the next state and update the current state.
 *          Perform doAction() according to the current state no matter if
 *          a transition has occured or not.

 * @return  None
 */
void SM_runMachine(void)
{
    static state_t currentState, nextState;

    if (checkStateTransition(currentState, &nextState)) // transistion has occurred
    {
        if (stateEntry[currentState].exitAction != NULL)
            stateEntry[currentState].exitAction();
        if (stateEntry[nextState].entryAction != NULL)
            stateEntry[nextState].entryAction();
        currentState = nextState; // update the state
    }
    if (stateEntry[currentState].doAction != NULL)
        stateEntry[currentState].doAction(); // run main action of the current state
}