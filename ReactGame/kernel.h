#ifndef __KERNEL__
#define __KERNEL__

#include "Arduino.h" 

/* general state management procedure */

void updateStateTime();
void changeState(int new_state);

int getCurrentState();
long getCurrentTimeInState();
bool isJustEnteredInState();

#endif
