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

/* core business logic  */

void initCore();
void intro();
void sleep();
void start();
void ingame();
void result();
void gameOver();

#endif
