#ifndef __OUTPUT__
#define __OUTPUT__

#define WINNER_STRING_INDEX 2
#define MAX_CHAR_ON_SCREEN 16

enum MessageType {
  WELCOME_STR = 0,
  GAMEOVER_TIE_STR = 1,
  GAMEOVER_WINNER_STR = 2
};

void initOutput();
void showIntroScreen();
void showStartScreen();

void resetOutput();
void rotateCharOnScreen(const char* str);
void fadeLed();
void printValue(int N);
int getGreenLed(int player);
int getRedLed(int player);
void changeLedState(int player, int point, int state);
const char* getGameMessage(MessageType type);
void updateWinnerString(int winner);
void printLevelDifficulty(char choice);
 
#endif
