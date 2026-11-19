#include "HWPlatform.h"
#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"
#include "config.h"
#include "devices/leds/Led.h"
#include "devices/buttons/ButtonImpl.h"
#include "devices/servo/ServoMotorImpl.h"
#include "devices/sonar/Sonar.h"
#include "devices/temp/TempSensorTMP36.h"
#include "devices/potentiometer/Pot.h"

void wakeUp(){}

HWPlatform::HWPlatform(){
  pStartButton = new ButtonImpl(START_PIN);
  pStopButton = new ButtonImpl(STOP_PIN);
  pResetButton = new ButtonImpl(RESET_PIN);

  pWorkingLed = new Led(WORKING_LED_PIN);
  pAlarmLed = new Led(ALARM_LED_PIN);

  pPotentiometer = new Potentiometer(POT_PIN);
  pServo = new ServoMotorImpl(SERVO_PIN);
  pSonar = new Sonar(ECHO_PIN, TRIG_PIN, SONAR_MAX_DIST); // DA DEFINIRE, MAX TIME DIPENDE DA TEMPERATURA, DISTANZA E VELOCITA' SERVO
  pTempSensor = new TempSensorTMP36(TEMP_PIN);
}

void HWPlatform::init(){
  pSonar->setTemperature(pTempSensor->getTemperature());
}

Button* HWPlatform::getStartButton(){
  return this->pStartButton;
}

Button* HWPlatform::getStopButton(){
  return this->pStopButton;
}

Button* HWPlatform::getResetButton(){
  return this->pResetButton;
}

Led*  HWPlatform::getWorkingLed(){
  return this->pWorkingLed;
}

Led*  HWPlatform::getAlarmLed(){
  return this->pAlarmLed;
}

ServoMotor* HWPlatform::getMotor(){
  return this->pServo;
}

Sonar* HWPlatform::getSonar(){
  return this->pSonar;
}

TempSensor* HWPlatform::getTempSensor(){
  return this->pTempSensor;
}

Potentiometer* HWPlatform::getPotentiometer() {
  return this->pPotentiometer;
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

