#include "output.h"
#include "Arduino.h"
#include "config.h"
#include "kernel.h"
#include "core.h"

#include <LiquidCrystal_I2C.h>

#define BOUNCING_TIME 100

const long intervalLCD = 400;
const long intervalLed = 400;

const char startString[] = "Welcome to the React! Game. Press Buttons to Start ";
const int lenght = sizeof(startString) - 1;
const int displayLenght = 16;

unsigned long currentTime;
unsigned long previousTime = 0;
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
};

void initOutput(){
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(outputPins[i], OUTPUT);     
  }

  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2);
  lcd.init();
  lcd.backlight();

  if (getCurrentState() == INTRO_STATE) {
    lcd.setCursor(5, 1);
    lcd.print("React!");
  }
}

void printCharOnScreen() {
  currentTime = getCurrentTimeInState();

  if (currentMillis - lastUpdate >= interval) {
    lastUpdate = currentMillis;

    lcd.setCursor(0, 0);
    for (int c = 0; c < displayLenght; c++) {
      int currentPos = (pos + c) % lenght;
      lcd.print(startString[currentPos]);
    }
    pos = (pos + 1) % lenght;
  }
}

void fadeLed() {
  currentTime = getCurrentState(); 

  if (currentTime - previousTime >= intervalLed) {
    analogWrite(P1_RED_LED_PIN, brightness1);
    analogWrite(P2_RED_LED_PIN, brightness2);
  
    brightness1 += fadeAmount;
    brightness2 -= fadeAmount;

    if (brightness1 <= 0 || brightness1 >= 255) {
      fadeAmount = -fadeAmount;
    }
  }
}

void resetOutput() {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(outputPins[i], LOW);
    lcd.noBacklight();   
  }
}




