#ifndef __SONAR__
#define __SONAR__

#include "PresenceSensor.h"
#include "ProximitySensor.h"

#define NO_OBJ_DETECTED -1

class Sonar: public ProximitySensor, public PresenceSensor {

public:  
  Sonar(int echoPin, int trigPin, long maxDistance);
  float getDistance();
  bool isDetected(); // funziona solo dopo la prima misurazione, da capire come implementarlo
  void setTemperature(float temp);  

private:
    float getSoundSpeed();
    long getTimeout();
    
    float temperature;    
    bool detected;
    int echoPin, trigPin;
    long timeOut;
    float maxDistance;
};

#endif 
