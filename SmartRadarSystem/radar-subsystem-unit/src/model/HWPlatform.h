#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/buttons/Button.h"
#include "devices/leds/Led.h"
#include "devices/potentiometer/Pot.h"
#include "devices/servo/ServoMotor.h"
#include "devices/sonar/Sonar.h"
#include "devices/temp/TempSensor.h"

class HWPlatform {

public:
  HWPlatform();
  void init();
  void test();

  Button* getStartButton();
  Button* getStopButton();
  Button* getResetButton();
  Led*  getWorkingLed();
  Led*  getAlarmLed();
  ServoMotor* getMotor();
  Sonar* getSonar();
  TempSensor* getTempSensor();
  Potentiometer* getPotentiometer();

private:
  Button* pStartButton;
  Button* pStopButton;
  Button* pResetButton;
  Led* pWorkingLed;
  Led* pAlarmLed;
  ServoMotor* pServo;
  Sonar* pSonar;
  TempSensor* pTempSensor;
  Potentiometer* pPotentiometer;
};

#endif
