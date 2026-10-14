#ifndef __INPUT__
#define __INPUT__

void initInput();
void resetInput();
bool isButtonPressed(int buttonIndex);
bool isAtLeastOneButtonPressed();
bool allButtonPressed();
int getNumberOfPress(int player);
void changePoint(int player, int point);
bool isTie();
int getWinner();
void resetPoint();
 
#endif
