#ifndef STATE_MACHINE_H__
#define STATE_MACHINE_H__

#include <Arduino.h>

/* List of all states */
typedef enum
{
    S_IDLE, // default state
    S_DEC,
    S_INC,
    S_RUN
} state_t;

/* List of all events */
typedef enum
{
    E_BTN_DEC,
    E_BTN_INC,
    E_BTN_START_STOP,
    NO_EVENT
} event_t;

#endif