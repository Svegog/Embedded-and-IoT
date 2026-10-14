#include "core.h"
#include "Arduino.h"
#include "kernel.h"
#include "input.h"
#include "output.h"

#include <avr/sleep.h>
#include <avr/power.h>

#define MAX_CHAR_ON_SCREEN 16
#define POINT 1
#define PENALTY -1

#define MAX_TIME_IN_INTRO_STATE 10000
#define DELAY_BEFORE_GAME 3000
#define MAX_WAIT 5000

bool gamestarted = false;
long T1 = 5000;
long waitTime;
int N;

/* core business logic */

void initCore(){
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}

void intro(){
  if (isJustEnteredInState()){
    Serial.println("Intro...");
    resetInput();
  }

  long dt = getCurrentTimeInState();

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
    initOutput();
    resetInput();
  }

  long dt = getCurrentTimeInState();

  if (dt > DELAY_BEFORE_GAME){
    changeState(INGAME_STATE);
  }
}

void ingame() {
  if (isJustEnteredInState()) {
    waitTime = random(0, MAX_WAIT + 1);
    gamestarted = false;
    resetInput();
    resetOutput();
  }

  long dt = getCurrentTimeInState();

  if (!gamestarted && dt > waitTime) {
    gamestarted = true;
    N = random(1, 4);
    printValue(N);
  }

  for (int i = 0; i < NUM_PLAYERS; i++) {
    int presses = getNumberOfPress(i);
    if (presses == 0) continue;

    if (!gamestarted || presses > N) {
      int value = PENALTY;
      // penalty to i TODO
    } else if (presses == N) {
      int value = PENALTY;
      // i win TODO
    }

    changeState(RESULT_STATE);
    changePoint(i, value);
    turnOnLEd(i, value);
  }

  if (dt > T1) {
      changeState(GAMEOVER_STATE); // timer for T1 end, game end TODO
    }
}

void result() {}

void finalize(){
  if (isJustEnteredInState()){
    Serial.println("Finalize...");
  }
  changeState(INTRO_STATE);
}

void enterSleepState() {}