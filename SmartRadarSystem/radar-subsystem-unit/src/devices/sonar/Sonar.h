#ifndef __SONAR__
#define __SONAR__

#include "PresenceSensor.h"
#include "ProximitySensor.h"

#define NO_OBJ_DETECTED -1

class Sonar: public ProximitySensor, public PresenceSensor {

public:  
  Sonar(int echoPin, int trigPin, long maxTime);
  float getDistance();
  bool isDetected();
  void setTemperature(float temp);  

private:
    const float vs = 331.5 + 0.6*20;
    float getSoundSpeed();
    
    float temperature;    
    bool detected;
    int echoPin, trigPin;
    long timeOut;
};

#endif 
