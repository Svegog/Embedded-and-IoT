#include "core.h"
#include "Arduino.h"
#include "kernel.h"
#include "input.h"
#include "output.h"

#include <avr/sleep.h>
#include <avr/power.h>

#define MAX_CHAR_ON_SCREEN 16

#define MAX_TIME_IN_INTRO_STATE 10000

const long intervalLCD = 400;
const long intervalLCD = 400;

const char startString[] = "Welcome to the React! Game. Press Buttons to Start ";
const int lenght = sizeof(startString) - 1;
const int displayLenght = 16;

unsigned long currentTime;
unsigned long previousTime = 0;
int pos = 0;

int brightness1 = 0;
int brightness2 = 255;
int fadeAmount = 5;

/* core business logic */

void initCore(){
  Serial.begin(9600);
}

void intro(){
  if (isJustEnteredInState()){
    Serial.println("Intro...");
    resetInput();
  }

  int dt = getCurrentTimeInState();

  if (dt > MAX_TIME_IN_INTRO_STATE){
    changeState(SLEEP_STATE);
  }

  printCharOnScreen();
  fadeLed();
  
  if (allButtonPressed()) {
    changeState(START_STATE);
  }
}

void sleep(){
  if (isJustEnteredInState()){
    Serial.println("Sleep...");
  }

  resetOutput();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  power_all_disable(); // disable all periferal 
  sleep_mode(); // sleep mode
  
  sleep_disable(); // after interrupt 
  power_all_enable();

  initOutput();

  // change the state if one button is pressed
  if (isAtLeastOneButtonPressed()){
    changeState(INTRO_STATE);          
  }
}

void start(){
  if (isJustEnteredInState()){
    Serial.println("Start...");
    resetInput();
  }

  /* change the state if button 0 is pressed */
  if (isButtonPressed(0)){
    changeState(STAGE2_STATE);          
  }
}

void ingame() {}

void finalize(){
  if (isJustEnteredInState()){
    Serial.println("Finalize...");
  }
  changeState(INTRO_STATE);
}

void enterSleepState() {}