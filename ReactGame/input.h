#ifndef __INPUT__
#define __INPUT__

void initInput();
void resetInput();
bool isButtonPressed(int buttonIndex);
bool allButtonPressed();
int getNumberOfPress(int player);
int getSensorValue();
 
#endif
