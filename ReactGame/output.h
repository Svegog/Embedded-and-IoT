#ifndef __OUTPUT__
#define __OUTPUT__

enum MessageType {
  WELCOME_STR = 0,
  GAMEOVER_TIE_STR = 1,
  GAMEOVER_WINNER_STR = 2
};

void initOutput();
void backlightOn();
void showIntroScreen();
void showStartScreen();
void showResultScreen(int p1, int p2);

void resetOutput();
void showMessage(MessageType type);
void fadeLed();
void printValue(int N);
void changeLedState(int player, int point, int state);
void updateWinnerString(int winner);
void printLevelDifficulty(int choice);
 
#endif
