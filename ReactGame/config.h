#ifndef __CONFIG__
#define __CONFIG__

// HARDWARE AND GAME CONFIG

#define P1 0
#define P2 1

#define NUM_PLAYERS 2
#define NUM_BUTTONS 2
#define NUM_LEDS 4

#define P1_BUT_PIN 4
#define P2_BUT_PIN 9

#define P1_RED_LED_PIN 5
#define P1_GREEN_LED_PIN 7
#define P2_RED_LED_PIN 6
#define P2_GREEN_LED_PIN 8

#define POTENT_PIN A0

// CORE CONFIG

#define POINT 1
#define PENALTY -1

#define T1_INITIAL 5000
#define T1_MIN 500

#define POTENTIOMETER_DELAY 50
#define REDUCTION_MULTIPLIER 50

#define MAX_TIME_IN_INTRO_STATE 15000 // more time to choose difficulty
#define DELAY_BEFORE_GAME 3000
#define MAX_WAIT 5000
#define DELAY_BEFORE_NEW_ROUND 4000
#define DELAY_GAME_OVER 2000
#define DELAY_BEFORE_RESTART 5000

#endif
