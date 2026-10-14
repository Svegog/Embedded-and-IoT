#ifndef __OUTPUT__
#define __OUTPUT__

enum MessageType {
  WELCOME_STR = 0,
  GAMEOVER_TIE_STR = 1,
  GAMEOVER_WINNER_STR = 2
};

void initOutput();
void resetOutput();
void rotateCharOnScreen(const char* str);
void fadeLed();
void printValue(int N);
int getGreenLed(int player);
int getRedLed(int player);
void changeLedState(int player, int point, int state);
const char* getGameMessage(MessageType type);
void updateWinnerString(int winner);
 
#endif
