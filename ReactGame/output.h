#ifndef __OUTPUT__
#define __OUTPUT__

void initOutput();
void resetOutput();
void rotateCharOnScreen();
void fadeLed();
void printValue(int N);
int getGreenLed(int player);
int getRedLed(int player);
void changeLedState(int player, int point, int state);
 
#endif
