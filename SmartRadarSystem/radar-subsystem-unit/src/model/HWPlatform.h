#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/buttons/Button.h"
#include "devices/leds/Led.h"
#include "devices/servo/ServoMotor.h"

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

private:
  Button* startButton;
  Button* stopButton;
  Button* resetButton;
  Led* workingLed;
  Led* alarmLed;
  ServoMotor* pMotor;
  
};

#endif
