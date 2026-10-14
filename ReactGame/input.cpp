#include "input.h"
#include "Arduino.h"
#include "config.h"
#include "kernel.h"
#include "core.h"

#include <EnableInterrupt.h>

#define BOUNCING_TIME 100

// #define __DEBUG__

uint8_t inputPins[NUM_BUTTONS] = {P1_BUT_PIN, P2_BUT_PIN};
volatile int numberOfPress[NUM_PLAYERS];
int points[NUM_PLAYERS];
bool buttonPressed[NUM_BUTTONS] = {false, false};

/* used for debouncing */
long lastButtonPressedTimestamps[NUM_BUTTONS];

void buttonHandler(int i);
void buttonHandlerP1(){ buttonHandler(P1); }
void buttonHandlerP2(){ buttonHandler(P2); }

void (*buttonHandlers[NUM_BUTTONS])() = { buttonHandlerP1, buttonHandlerP2 };

void buttonHandler(int i){
  unsigned long ts = millis();
  int status = digitalRead(inputPins[i]);

  if (status == HIGH && !buttonPressed[i] && 
    (ts - lastButtonPressedTimestamps[i] > BOUNCING_TIME)) { 
      buttonPressed[i] = true;
      numberOfPress[i] += 1;
      lastButtonPressedTimestamps[i] = ts;
  } 
  else if (status == LOW && buttonPressed[i] && 
    (ts - lastButtonPressedTimestamps[i] > BOUNCING_TIME)) {
      buttonPressed[i] = false;
      lastButtonPressedTimestamps[i] = ts;
  }
}

void initInput(){
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(inputPins[i], INPUT); 
    enableInterrupt(inputPins[i], buttonHandlers[i], CHANGE);
  }
}

void resetInput(){
  long ts = millis();
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttonPressed[i] = false;
    numberOfPress[i] = 0;      
    lastButtonPressedTimestamps[i] = ts;    
  }
}

bool isButtonPressed(int buttonIndex){
  return buttonPressed[buttonIndex];
}

bool hasBeenPressed(int index){
  return buttonPressed[index]; 
}

bool isAtLeastOneButtonPressed() {
  bool result = false;
  for (int i = 0; i < NUM_BUTTONS; i++) {
    result = result || isButtonPressed(i);
  }
  return result;
}

bool allButtonPressed() {
  bool result = true;
  for (int i = 0; i < NUM_BUTTONS; i++) {
    result = result && isButtonPressed(i);
  }
  return result;
}

int getNumberOfPress(int player){
  noInterrupts();
  int n = numberOfPress[player];
  interrupts();
  return n;
}

void changePoint(int player, int point) {
  points[player] += point;
}

// we NEED at least two player, fair enough because this is not a single player game lol
bool isTie() {
  int startValue = points[0]
  for (int i = 1; i < NUM_PLAYERS; i++) {
    if (startValue != points[i]) return false;
  }
  return true;
}

int getWinner() {
  int resultIndex;
  int max = 0; // max cannot be 0, the game should end if nobody press a button
  for (int i = 0; i < NUM_PLAYERS; i++) {
    if (points[i] > max) {
      max = points[i];
      resultIndex = i;
    }
  }
  return resultIndex + 1; 
}

void resetPoint() {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    points[i] = 0;
  }
}



