#include <avr/sleep.h>
#include <avr/power.h>

#include "Arduino.h"
#include "core.h"
#include "kernel.h"
#include "input.h"
#include "output.h"
#include "score.h"
#include "config.h"

#define BAUD_RATE 9600
#define MIN_POT_VALUE 0
#define MAX_POT_VALUE 1024
#define MINIMUM_DIFFICULTY 1
#define MAXIMUM_DIFFICULTY 5 // need to be 1 over the real maximum difficulty so the map function can also output value 4

static void decreaseTime();
static bool updateRoundResult();

static bool gamestarted = false;
static bool endStringSet = false;

static unsigned long T1 = T1_INITIAL;
static unsigned long waitTime;
static unsigned long reductionFactor;
static int N;

static unsigned long previousTimePot;

static int roundScores[NUM_PLAYERS];

void printStartSection() {
  Serial.println("Choose difficulty with potentiometer");
  Serial.println("----------------------------------------------");
  Serial.println("A letter will be print on screen, meaning:");
  Serial.println("    E for easy");
  Serial.println("    M for medium");
  Serial.println("    H for hard");
  Serial.println("    S for superhard");
  Serial.println("----------------------------------------------");
}

void initCore(){
  Serial.begin(BAUD_RATE);
  randomSeed(analogRead(SEED_PIN));
  reductionFactor = 0;
  initOutput();
  initInput();
  Serial.println("Boot...");
}

void intro(){
  if (isJustEnteredInState()){
    Serial.println("Intro...");
    printStartSection();
  
    showIntroScreen();
    resetInput();

    previousTimePot = 0;
  }

  unsigned long dt = getCurrentTimeInState();

  if (dt > MAX_TIME_IN_INTRO_STATE){
    changeState(SLEEP_STATE);
    return;
  }

  rotateMessage(WELCOME_STR);
  fadeLed();
  
  if (dt - previousTimePot > POTENTIOMETER_DELAY) {
    
    previousTimePot = dt;
    int sensorValue = getSensorValue();
    int choice = map(
      sensorValue, 
      MIN_POT_VALUE, 
      MAX_POT_VALUE, 
      MINIMUM_DIFFICULTY, 
      MAXIMUM_DIFFICULTY
    );
    reductionFactor = REDUCTION_MULTIPLIER*choice;

    printLevelDifficulty(choice);
  }
  
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

  // change the state after the interrupt
  changeState(INTRO_STATE);         
}

void start(){
  if (isJustEnteredInState()){
    Serial.println("Start...");
    T1 = T1_INITIAL;
    resetOutput();
    showStartScreen();
    resetPoint();
  }

  unsigned long dt = getCurrentTimeInState();

  if (dt > DELAY_BEFORE_GAME){
    changeState(INGAME_STATE);
  }
}

void ingame() {
  if (isJustEnteredInState()) {
    Serial.println("In Game...");
    waitTime = random(0, MAX_WAIT + 1);
    gamestarted = false;
    for (int i = 0; i < NUM_PLAYERS; i++) {
      roundScores[i] = 0;
    }
    resetInput();
    resetOutput();
  }

  unsigned long dt = getCurrentTimeInState();

  for (int i = 0; i < NUM_PLAYERS; i++) {
    int presses = getNumberOfPress(i);

    if (presses != 0 && !gamestarted) {
      roundScores[i] = PENALTY;
    }
  }

  if (!gamestarted && dt > waitTime) {
    gamestarted = true;
    N = random(1, 4);
    printValue(N);
  }

  if (dt > (waitTime + T1) && gamestarted) {
    resetOutput();
    
    bool nobodyPressed = updateRoundResult();
    changePoint(roundScores);
    for (int i = 0; i < NUM_PLAYERS; i++){
      changeLedState(i, roundScores[i], HIGH);
    }
    decreaseTime();

    changeState(nobodyPressed ? GAMEOVER_STATE : RESULT_STATE);

    resetInput();
  }
}

static void decreaseTime() {
  if (T1 > T1_MIN + reductionFactor) {
    T1 -= reductionFactor;
  } else {
    T1 = T1_MIN;
  }
}

static bool updateRoundResult() {
  int numberOfPresses[NUM_PLAYERS];
  for (int i = 0; i < NUM_PLAYERS; i++) {
    numberOfPresses[i] = getNumberOfPress(i);
    if (numberOfPresses[i] == N && roundScores[i] != PENALTY) {
      roundScores[i] = POINT;
    } else if (numberOfPresses[i] > N) {
      roundScores[i] = PENALTY;
    }
  }

  if (roundScores[0] == POINT && roundScores[1] == POINT) {
    if (getLastPressTimestamp(0) < getLastPressTimestamp(1)) {
      roundScores[1] = 0;
    } else {
      roundScores[0] = 0;
    }
  }

  return (numberOfPresses[0] == 0 && numberOfPresses[1] == 0);
}

void result() {
  if (isJustEnteredInState()){
    Serial.println("Result...");
    showResultScreen(getPoint(P1), getPoint(P2));
  }

  unsigned long dt = getCurrentTimeInState();

  if (dt > DELAY_BEFORE_NEW_ROUND) {
    changeState(INGAME_STATE);
  }
}

void gameOver(){
  if (isJustEnteredInState()){
    Serial.println("Finalize...");
    resetOutput();
    backlightOn();
    for (int i = 0; i < NUM_PLAYERS; i++) {
      changeLedState(i, PENALTY, HIGH);
    }
  }

  if (getCurrentTimeInState() > DELAY_GAME_OVER) {
    for (int i = 0; i < NUM_PLAYERS; i++) {
      changeLedState(i, PENALTY, LOW);
    }
  }

  if (isTie()) {
    rotateMessage(GAMEOVER_TIE_STR);
  } else {
    if (!endStringSet) {
      int winner = getWinner();
      updateWinnerString(winner);
      endStringSet = true;
    }
    rotateMessage(GAMEOVER_WINNER_STR);
  }

  unsigned long dt = getCurrentTimeInState();
  if (dt > DELAY_BEFORE_RESTART) {
    changeState(INTRO_STATE);
    endStringSet = false; 
    // no need to reset the output.cpp string, because it will 
    // be overwritten after every game
  }
}