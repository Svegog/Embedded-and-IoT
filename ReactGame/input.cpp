#include "input.h"
#include "Arduino.h"
#include "config.h"

#include <EnableInterrupt.h>

#define BOUNCING_TIME 100

// #define __DEBUG__

uint8_t inputPins[NUM_BUTTONS] = {P1_BUT_PIN, P2_BUT_PIN};
int numberOfPress[NUM_PLAYERS] = {0,0};
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

void initInput(){
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(outputPins[i], OUTPUT); 
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




