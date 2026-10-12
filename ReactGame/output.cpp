#include "output.h"
#include "Arduino.h"
#include "config.h"

#include <LiquidCrystal_I2C.h>

#define BOUNCING_TIME 100

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
}




