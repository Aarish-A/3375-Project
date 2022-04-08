#pragma once
#include "constants.h"


// Structure to manage Private A9 Timer register structure
typedef struct _a9_timer {
    unsigned int load;
    unsigned int count;
    unsigned int control;
    unsigned int status;
} a9_timer;

// Initialize given timer to set status, operating mode, and load value
void init_timer(a9_timer* const timer);

// Wait for given timer to finish its current countdown
void wait_for_timer(a9_timer* const timer);


void init_timer(a9_timer* const timer) {
    // Set timer initial values
    timer->status = 1;
    timer->control = 0b0011;
    timer->load = INIT_LOAD;
}

void wait_for_timer(a9_timer* const timer) {
    // Poll status flag until it indicates timer is done countdown
    while (timer->status == 0);

    // Reset status for next countdown
    timer->status = 1;
}