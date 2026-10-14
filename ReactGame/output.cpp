#include "output.h"
#include "Arduino.h"
#include "config.h"
#include "kernel.h"
#include "core.h"

#include <LiquidCrystal_I2C.h>

#define BOUNCING_TIME 100

const long intervalLCD = 400;
const long intervalLED = 20;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2);

char* strings[] = {
  "Welcome to the React! Game. Press Buttons to Start ",
  "Game Over - Tie",
  "Game Over - The winner is XX"
};
const int displayLenght = 16;

unsigned long currentTimeLED;
unsigned long currentTimeLCD;

unsigned long previousTimeLED = 0;
unsigned long previousTimeLCD = 0;
int pos = 0;

int brightness1 = 0;
int brightness2 = 255;
int fadeAmount = 5;

// #define __DEBUG__

uint8_t outputPins[NUM_LEDS] = {
  P1_RED_LED_PIN, 
  P1_GREEN_LED_PIN, 
  P2_RED_LED_PIN,
  P2_GREEN_LED_PIN
}; // odd are always green

void initOutput(){
  int status = getCurrentState();

  if (status == INTRO_STATE) {
    for (int i = 0; i < NUM_LEDS; i++) {
      pinMode(outputPins[i], OUTPUT);     
    }

    lcd.init();
    lcd.setCursor(5, 1);
    lcd.print("React!");

  } else if (status == START_STATE) {
    lcd.setCursor(5, 0);
    lcd.print("Go!");
  }

  lcd.backlight();
}

void resetOutput() {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(outputPins[i], LOW); 
  }
  pos = 0;
  lcd.clear();
  lcd.noBacklight();  
}

void rotateCharOnScreen() {
  currentTimeLCD = millis();

  if (currentTimeLCD - previousTimeLCD >= intervalLCD) {
    previousTimeLCD = currentTimeLCD;

    lcd.setCursor(0, 0);
    for (int c = 0; c < displayLenght; c++) {
      int currentPos = (pos + c) % lenght;
      lcd.print(startString[currentPos]);
    }
    pos = (pos + 1) % lenght;
  }
}

void fadeLed() {
  currentTimeLED = millis(); 

  if (currentTimeLED - previousTimeLED >= intervalLED) {
    previousTimeLED = currentTimeLED;

    analogWrite(P1_RED_LED_PIN, brightness1);
    analogWrite(P2_RED_LED_PIN, brightness2);
  
    brightness1 += fadeAmount;
    brightness2 -= fadeAmount;

    if (brightness1 <= 0 || brightness1 >= 255) {
      fadeAmount = -fadeAmount;
    }
  }
}

void printValue(int N) {
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