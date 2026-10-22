#include <EnableInterrupt.h>

#include "Arduino.h"
#include "input.h"
#include "config.h"

#define BOUNCING_TIME 40 // both HIGH and LOW state

uint8_t inputPins[NUM_BUTTONS] = {P1_BUT_PIN, P2_BUT_PIN};
volatile int numberOfPress[NUM_PLAYERS];
volatile bool buttonPressed[NUM_BUTTONS] = {false, false};

/* used for debouncing */
volatile long lastButtonPressedTimestamps[NUM_BUTTONS];

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
  unsigned long ts = millis();
  noInterrupts();
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttonPressed[i] = false;
    numberOfPress[i] = 0;      
    lastButtonPressedTimestamps[i] = ts;    
  }
  interrupts();
}

bool isButtonPressed(int buttonIndex){
  return buttonPressed[buttonIndex];
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

int getSensorValue() {
  return analogRead(POTENT_PIN);
}



