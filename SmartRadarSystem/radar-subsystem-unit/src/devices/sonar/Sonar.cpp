#include "Sonar.h"
#include "Arduino.h"

Sonar::Sonar(int echoP, int trigP, float maxDistance) : echoPin(echoP), trigPin(trigP), maxDistance(maxDistance){
  temperature = 20; // default value
  timeOut = getTimeout();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  detected = false; // default value DA DECIDERE, usare getDistance() quando chiamato per capire se detected oppure lasciare così
}

void Sonar::setTemperature(float temp){
  temperature = temp;
  timeOut = getTimeout();
}
float Sonar::getSoundSpeed(){
  return 331.5 + 0.6*temperature;   
}

long Sonar::getTimeout() {
  return (long)(2.0 * maxDistance / getSoundSpeed() * 1000000.0);
}

bool Sonar::isDetected(){
  return detected;
}

float Sonar::getDistance(){
    digitalWrite(trigPin,LOW);
    delayMicroseconds(3);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin,LOW);
    
    float tUS = pulseIn(echoPin, HIGH, timeOut);
    if (tUS == 0) {
        detected = false;
        return NO_OBJ_DETECTED;
    } else {
        float t = tUS / 1000.0 / 1000.0 / 2;
        float d = t*getSoundSpeed();
        detected = true;
        return d;  
    }
}

