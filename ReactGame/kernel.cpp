#include "kernel.h"

/* current state */
static int currentState;

/* time registered when entering the new state */
static unsigned long enteredStateTime;

/* how long it the game is in current state */
static unsigned long currentTimeInState;

/* flag which is true if it is the first cycle in that state */
static bool firstCheckInState;

/* state management procedures */

int getCurrentState(){
  return currentState;
}

unsigned long getCurrentTimeInState(){
  return currentTimeInState;
}

bool isJustEnteredInState(){
  bool com = firstCheckInState;
  firstCheckInState = false;
  return com;
}

void changeState(int new_state){
  currentState = new_state;
  enteredStateTime = millis();
  firstCheckInState = true;
}

void updateStateTime(){
  currentTimeInState = millis() - enteredStateTime;
}