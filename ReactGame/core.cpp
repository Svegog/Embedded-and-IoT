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

#define T1_INITIAL 5000
#define T1_MIN 500

#define MAX_TIME_IN_INTRO_STATE 10000
#define DELAY_BEFORE_GAME 3000
#define MAX_WAIT 5000
#define DELAY_BEFORE_NEW_ROUND 2000
#define DELAY_BEFORE_RESTART 5000
#define DELAY_GAME_OVER 2000

bool gamestarted = false;
bool endStringSet = false;
long T1 = T1_INITIAL;
long waitTime;
long reductionFactor;
int N;

/* core business logic */

void initCore(){
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  reductionFactor = 0; // TODO
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
    return;
  }

  rotateCharOnScreen(getGameMessage(WELCOME_STR));
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

  Serial.flush();
  power_all_disable(); // disable all periferal 
  sleep_mode(); // sleep mode
  
  sleep_disable(); // after interrupt 
  power_all_enable();

  initOutput();

  // change the state after the interrupt
  changeState(INTRO_STATE);         
}

void start(){
  if (isJustEnteredInState()){
    Serial.println("Start...");
    T1 = T1_INITIAL;
    resetOutput();
    initOutput();
    resetInput();
    resetPoint();
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

    int value;
    if (!gamestarted || presses > N) {
      value = PENALTY;
    } else if (presses == N) {
      value = POINT;
    } else {
      continue;
    }

    changeState(RESULT_STATE);
    resetInput();
    resetOutput();
    changePoint(i, value);
    changeLedState(i, value, HIGH);
    T1 -= reductionFactor;
    if (T1 < T1_MIN) T1 = T1_MIN;
    return;
  }

  if (dt > (waitTime + T1) && gamestarted) {
      changeState(GAMEOVER_STATE);
  }
}

void result() {
  if (isJustEnteredInState()){
    Serial.println("Result...");
    initOutput();
  }

  long dt = getCurrentTimeInState();

  if (dt > DELAY_BEFORE_NEW_ROUND) {
    changeState(INGAME_STATE);
  }
}

void gameOver(){
  if (isJustEnteredInState()){
    Serial.println("Finalize...");
    resetOutput();
    initOutput();
    for (int i = 0; i < NUM_PLAYERS; i++) {
      changeLedState(i, PENALTY, HIGH);
    }
  }

  if (getCurrentTimeInState() > DELAY_GAME_OVER) {
    for (int i = 0; i < NUM_PLAYERS; i++) {
      changeLedState(i, PENALTY, LOW);
    };
  }

  if (isTie()) {
    rotateCharOnScreen(getGameMessage(GAMEOVER_TIE_STR));
  } else {
    if (!endStringSet) {
      int winner = getWinner();
      updateWinnerString(winner);
      endStringSet = true;
    }
    rotateCharOnScreen(getGameMessage(GAMEOVER_WINNER_STR));
  }

  long dt = getCurrentTimeInState();
  if (dt > DELAY_BEFORE_RESTART) {
    changeState(INTRO_STATE);
    endStringSet = false; 
    // no need to reset the output.cpp string, because it will 
    // be overwritten after every game
  }
}