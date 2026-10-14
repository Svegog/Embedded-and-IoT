#include "core.h"
#include "Arduino.h"
#include "kernel.h"
#include "input.h"
#include "output.h"
#include "config.h"

#include <avr/sleep.h>
#include <avr/power.h>

#define POINT 1
#define PENALTY -1

#define MAX_TIME_IN_INTRO_STATE 10000
#define DELAY_BEFORE_GAME 3000
#define MAX_WAIT 5000
#define DELAY_BEFORE_NEW_ROUND 2000
#define DELAY_BEFORE_RESTART 5000

const char* currentMessage = nullptr;

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
    initOutput();
    resetInput();
  }

  long dt = getCurrentTimeInState();

  if (dt > MAX_TIME_IN_INTRO_STATE){
    changeState(SLEEP_STATE);
  }

  rotateCharOnScreen(getMessage(WELCOME_STR));
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
    initOutput();
    printValue(N);
  }

  for (int i = 0; i < NUM_PLAYERS; i++) {
    int presses = getNumberOfPress(i);
    if (presses == 0) continue;

    if (!gamestarted || presses > N) {
      int value = PENALTY;
    } else if (presses == N) {
      int value = PENALTY;
    }

    changeState(RESULT_STATE);
    resetInput();
    resetOutput();
    changePoint(i, value);
    changeLedState(i, value, HIGH);
  }

  if (dt > T1) {
      changeState(GAMEOVER_STATE);
    }
}

void result() {
  if (isJustEnteredInState()){
    Serial.println("Result...");
  }

  long dt = getCurrentTimeInState();

  if (dt > DELAY_BEFORE_NEW_ROUND) {
    changeState(INGAME_STATE);
  }

}

void gameOver(){
  static char winnerMsgBuffer[40];

  if (isJustEnteredInState()){
    Serial.println("Finalize...");
    initOutput();
  }

  if (isTie()) {
    rotateCharOnScreen(GAMEOVER_TIE_STR);
  } else {
    int result = getWinner();
    updateWinnerString(result);
    rotateCharOnScreen(GAMEOVER_WINNER_STR);
  }

  long dt = getCurrentTimeInState();
  if (dt > DELAY_BEFORE_RESTART) {
    changeState(INTRO_STATE);
    resetPoint();
  }
}