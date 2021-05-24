#include "pid.h"
#include <PID_v1.h>
#include <Arduino.h>

double setpoint, input, output, windowSize;
unsigned long windowStartTime;
PID capulusPID(&input, &output, &setpoint, 2, 5, 1, DIRECT);

CAPULUS_PID::CAPULUS_PID(double ws){
    windowStartTime = millis();
    windowSize = ws;
    capulusPID.SetOutputLimits(0, windowSize);
    capulusPID.SetMode(AUTOMATIC);
}

void CAPULUS_PID::setCurrent(double temp){input = temp;}

void CAPULUS_PID::setTarget(double temp){setpoint = temp;}

bool CAPULUS_PID::signal(){
  capulusPID.Compute();
  unsigned long now = millis();
  if (now - windowStartTime > windowSize) windowStartTime += windowSize;
  return output > (now - windowStartTime);
}

//TODO add autotune https://playground.arduino.cc/Code/PIDAutotuneLibrary/