#ifndef __CORE__
#define __CORE__

#define EASY 1
#define MEDIUM 2
#define HARD 3
#define SUPER 4

#define INTRO_STATE   1
#define START_STATE  2
#define SLEEP_STATE  3
#define INGAME_STATE   4
#define RESULT_STATE 5
#define GAMEOVER_STATE 6 

#define POINT 1
#define PENALTY -1

#define T1_INITIAL 5000
#define T1_MIN 500

#define POTENTIOMETER_DELAY 50
#define REDUCTION_MULTIPLIER 50

#define MAX_TIME_IN_INTRO_STATE 15000 // more time to choose difficulty
#define DELAY_BEFORE_GAME 3000
#define MAX_WAIT 5000
#define DELAY_BEFORE_NEW_ROUND 4000
#define DELAY_GAME_OVER 2000
#define DELAY_BEFORE_RESTART 5000

/* core business logic  */

void initCore();
void intro();
void sleep();
void start();
void ingame();
void result();
void gameOver();

#endif
