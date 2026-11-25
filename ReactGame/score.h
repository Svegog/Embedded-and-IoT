#ifndef SCORE_H
#define SCORE_H

#include "config.h"

void resetPoint();
void changePoint(int scores[NUM_PLAYERS]);
int getPoint(int player);
bool isTie();
int getWinner();

#endif
