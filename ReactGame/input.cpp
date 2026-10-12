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
bool buttonPressed[NUM_BUTTONS] = {false, false};

/* used for debouncing */
long lastButtonPressedTimestamps[NUM_BUTTONS];

void buttonHandler(int i);
void buttonHandlerP1(){ buttonHandler(P1); }
void buttonHandlerP2(){ buttonHandler(P2); }

void (*buttonHandlers[NUM_BUTTONS])() = { buttonHandlerP1, buttonHandlerP2 };

void buttonHandler(int i){
  long ts = millis();
  if (ts - lastButtonPressedTimestamps[i] > BOUNCING_TIME){
    lastButtonPressedTimestamps[i] = ts;
    int status = digitalRead(inputPins[i]);
    if (status == HIGH && !buttonPressed[i]) { 
        buttonPressed[i] = true;
        numberOfPress[i] += 1;
    }
  }
}

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



