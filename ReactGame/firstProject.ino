#include "kernel.h"
#include "config.h"
#include "core.h"
#include "input.h" 
#include "output.h"

void setup() {
  initCore();
  initInput();
  changeState(INTRO_STATE);
}

void loop(){ 
  updateStateTime(); 
  switch (getCurrentState()) { 
  case INTRO_STATE:
    intro();
    break;
  case START_STATE:
    start();
    break;
  case SLEEP_STATE:
    sleep();
    break;
  case INGAME_STATE:
    ingame();
    break;
  case RESULT_STATE:
    result();
    break;
  case GAMEOVER_STATE:
    gameOver();
    break;
  }
}