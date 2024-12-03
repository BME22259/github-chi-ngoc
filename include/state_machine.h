#ifndef STATE_MACHINE_H__
#define STATE_MACHINE_H__

#include <Arduino.h>

/* Type definitions ---------------------- */ 
/* List of all states */
typedef enum
{
    S_IDLE, // default state
    S_RUN,
    S_INC,
    S_DEC,
    NUM_STATES
} state_t;

/* List of all events */
typedef enum
{
    E_BTN_DEC,
    E_BTN_INC,
    E_BTN_START_STOP,
    NO_EVENT
} event_t;

typedef void (*stateFunction_t)(void);

struct sStateAction
{
    stateFunction_t entryAction;
    stateFunction_t doAction;
    stateFunction_t exitAction;
};

extern struct sStateAction stateEntry[NUM_STATES];


/* List of all states */

void SM_runMachine(void);
void SM_createState(state_t stateName,
                  stateFunction_t entryFunction,
                  stateFunction_t doFunction,
                  stateFunction_t exitFunction);
#endif