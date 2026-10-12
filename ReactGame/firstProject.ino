#include "kernel.h"
#include "config.h"
#include "core.h"
#include "input.h" 
#include "output.h"

void setup() {
  initCore();
  initInput();
  initOutput();

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
  case GAMEOVER_STATE:
    finalize();
    break;
  }
}