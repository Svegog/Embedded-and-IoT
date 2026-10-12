#ifndef __CORE__
#define __CORE__

#define INTRO_STATE   1
#define START_STATE  2
#define SLEEP_STATE  3
#define INGAME_STATE   4
#define GAMEOVER_STATE 5 

/* core business logic  */

void initCore();
void intro();
void start();
void sleep();
void ingame();
void finalize();

#endif
