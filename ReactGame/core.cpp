#include "core.h"
#include "Arduino.h"
#include "kernel.h"
#include "input.h"
#include "output.h"

#define MAX_CHAR_ON_SCREEN 15
#define MAX_TIME_IN_INTRO_STATE 10000
#define MAX_TIME_IN_STAGE2_STATE 10000

char welcome_phrase[] = "Welcome to the React! Game. Press Buttons to Start";
char display_buffer[17] = "";
int lenght_phrase = strlen(welcome_phrase);

/* core business logic */

void initCore(){
  Serial.begin(9600);
}

void intro(){
  

  int dt = getCurrentTimeInState();
  if (dt > MAX_TIME_IN_INTRO_STATE){
    changeState(SLEEP_STATE);
  }
}

void stage1(){
  if (isJustEnteredInState()){
    Serial.println("Stage1...");
    resetInput();
  }

  /* change the state if button 0 is pressed */
  if (isButtonPressed(0)){
    changeState(STAGE2_STATE);          
  }
}

void stage2(){
  if (isJustEnteredInState()){
    Serial.println("Stage2...");
  }

  /* change the state if button 1 is pressed or max time elapsed*/
  if (isButtonPressed(1) || getCurrentTimeInState() > MAX_TIME_IN_STAGE2_STATE){
    changeState(FINAL_STATE);          
  }
}

void finalize(){
  if (isJustEnteredInState()){
    Serial.println("Finalize...");
  }
  changeState(INTRO_STATE);
}
