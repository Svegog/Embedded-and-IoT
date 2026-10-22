#include "Arduino.h"
#include "output.h"
#include "config.h"

#include <LiquidCrystal_I2C.h>

#define WINNER_STRING_INDEX 2
#define MAX_CHAR_ON_SCREEN 16

const long intervalLCD = 200;
const long intervalLED = 30;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2);

char strings[3][52] = { // hardcoded because message are fixed
  "Welcome to the React! Game. Press Buttons to Start ",
  "Game Over - Tie ",
  "Game Over - The winner is XX "
};

unsigned long currentTimeLED;
unsigned long currentTimeLCD;

unsigned long previousTimeLED = 0;
unsigned long previousTimeLCD = 0;
int pos = 0;

int brightness1 = 0;
int brightness2 = 255;
int fadeAmount = 5;

uint8_t outputPins[NUM_LEDS] = {
  P1_RED_LED_PIN, 
  P1_GREEN_LED_PIN, 
  P2_RED_LED_PIN,
  P2_GREEN_LED_PIN
}; // odd are always green

void initOutput() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(outputPins[i], OUTPUT);     
  }
}

void showIntroScreen() {
  lcd.init();
  lcd.setCursor(5, 1);
  lcd.print("React!");
  backlightOn();
}

void showStartScreen() {
  backlightOn();
  lcd.setCursor(7, 0);
  lcd.print("Go!");
}

void showResultScreen(int player1, int player2) {
  backlightOn();

  lcd.setCursor(3, 0);
  lcd.print("P1");
  lcd.setCursor(3, 1);
  lcd.print(player1);

  lcd.setCursor(11, 0);
  lcd.print("P2");
  lcd.setCursor(11, 1);
  lcd.print(player2);
}

void backlightOn() {
  lcd.backlight();
}

void resetOutput() {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(outputPins[i], LOW); 
  }
  lcd.clear();
  lcd.noBacklight();  
}

void rotateCharOnScreen(const char* str) {
  currentTimeLCD = millis();

  if (currentTimeLCD - previousTimeLCD < intervalLCD) {
    return;
  }

  previousTimeLCD = currentTimeLCD;

  if (str == NULL) return; // basic error handler
  int len = strlen(str);
  if (len == 0) return;

  // reset cursor if string change
  static const char* lastStr = NULL;
  if (str != lastStr) {
    pos = 0;
    lastStr = str;
  }

  lcd.setCursor(0, 0);
  for (int c = 0; c < MAX_CHAR_ON_SCREEN; c++) {
    int currentPos = (pos + c) % len;
    lcd.print(str[currentPos]);
  }
  
  pos = (pos + 1) % len;
}

void fadeLed() {
  currentTimeLED = millis(); 

  if (currentTimeLED - previousTimeLED >= intervalLED) {
    previousTimeLED = currentTimeLED;

    analogWrite(getRedLed(P1), brightness1);
    analogWrite(getRedLed(P2), brightness2);
  
    brightness1 += fadeAmount;
    brightness2 -= fadeAmount;

    if (brightness1 <= 0 || brightness1 >= 255) {
      fadeAmount = -fadeAmount;
    }
  }
}

void printValue(int N) {
  backlightOn();
  lcd.setCursor(8, 0);
  lcd.print(N);
}

int getGreenLed(int player) {
  return outputPins[player*2 + 1];
}

int getRedLed(int player) {
  return outputPins[player*2];
}

void changeLedState(int player, int point, int state) {
  int value = (state > 0) ? HIGH : LOW;
  int pin = (point > 0) ? getGreenLed(player) : getRedLed(player);
  digitalWrite(pin, value);
}

const char* getGameMessage(MessageType type) {
  return strings[type];
}

void updateWinnerString(int winner) {
  strings[WINNER_STRING_INDEX][26] = 'P';
  strings[WINNER_STRING_INDEX][27] = '0' + winner;
}

void printLevelDifficulty(char choice) {
  lcd.setCursor(15, 1);
  lcd.print(choice);
}
