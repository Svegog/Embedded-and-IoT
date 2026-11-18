#include "HWPlatform.h"
#include <Arduino.h>
#include "devices/buttons/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/leds/Led.h"
#include "devices/servo/ServoMotorImpl.h"
#include "kernel/Logger.h"

void wakeUp(){}

HWPlatform::HWPlatform(){
  startButton = new ButtonImpl(START_PIN);
  stopButton = new ButtonImpl(STOP_PIN);
  resetButton = new ButtonImpl(RESET_PIN);

  workingLed = new Led(WORKING_LED_PIN);
  alarmLed = new Led(ALARM_LED_PIN);
  
  pMotor = new ServoMotorImpl(SERVO_PIN);
}

void HWPlatform::init(){
}

Button* HWPlatform::getStartButton(){
  return this->startButton;
}

Button* HWPlatform::getStopButton(){
  return this->stopButton;
}

Button* HWPlatform::getResetButton(){
  return this->resetButton;
}

Led*  HWPlatform::getWorkingLed(){
  return this->workingLed;
}

Led*  HWPlatform::getAlarmLed(){
  return this->alarmLed;
}

ServoMotor* HWPlatform::getMotor(){
  return this->pMotor;
}

void HWPlatform::test(){
  // bool btPressed = pButton->isPressed();
  // pLed->switchOn();
  // pMotor->on();
  // pMotor->setPosition(90);
  // Logger.log("Button: " + String(btPressed ? "pressed" : " not pressed"));
  // delay(1000);
  // pMotor->setPosition(0);
  // delay(1000);
  // pMotor->off();
  // pLed->switchOff();
}

