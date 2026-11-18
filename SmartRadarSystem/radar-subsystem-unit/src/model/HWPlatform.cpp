#include "HWPlatform.h"
#include <Arduino.h>
#include "devices/buttons/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/leds/Led.h"
#include "devices/servo/ServoMotorImpl.h"
#include "devices/sonar/Sonar.h"
#include "devices/temp/TempSensorTMP36.h"
#include "devices/potentiometer/Pot.h"
#include "kernel/Logger.h"

void wakeUp(){}

HWPlatform::HWPlatform(){
  startButton = new ButtonImpl(START_PIN);
  stopButton = new ButtonImpl(STOP_PIN);
  resetButton = new ButtonImpl(RESET_PIN);

  workingLed = new Led(WORKING_LED_PIN);
  alarmLed = new Led(ALARM_LED_PIN);

  potentiometer = new Potentiometer(POT_PIN);
  servo = new ServoMotorImpl(SERVO_PIN);
  sonar = new Sonar(ECHO_PIN, TRIG_PIN, 6000); // DA DEFINIRE, MAX TIME DIPENDE DA TEMPERATURA, DISTANZA E VELOCITA' SERVO
  tempSensor = new TempSensorTMP36(TEMP_PIN);
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
  return this->servo;
}

Sonar* HWPlatform::getSonar(){
  return this->sonar;
}

TempSensor* HWPlatform::getTempSensor(){
  return this->tempSensor;
}

Potentiometer* HWPlatform::getPotentiometer() {
  return this->potentiometer;
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

