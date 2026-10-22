#include "score.h"
#include "config.h"

static int points[NUM_PLAYERS];

void resetPoint() {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    points[i] = 0;
  }
}

void changePoint(int player, int point) {
  points[player] += point;
}

int getPoint(int player) {
  return points[player];
}

// we NEED at least two player, fair enough because this is not a single player game lol
bool isTie() {
  int startValue = points[0];
  for (int i = 1; i < NUM_PLAYERS; i++) {
    if (startValue != points[i]) return false;
  }
  return true;
}

// returns the winner as 1-based number (P1 = 1, P2 = 2)
int getWinner() {
  int resultIndex = 0;
  int max = points[0];
  for (int i = 1; i < NUM_PLAYERS; i++) {
    if (points[i] > max) {
      max = points[i];
      resultIndex = i;
    }
  }
  return resultIndex + 1;
}
