#include "pid.h"
#include <PID_v1.h>

double setpoint, input, output, windowSize;
unsigned long windowStartTime;
PID capulusPID(&input, &output, &setpoint, 2, 5, 1, DIRECT);

CAPULUS_PID::CAPULUS_PID(){
    windowStartTime = 0;
    windowSize = 300;
    capulusPID.SetOutputLimits(0, windowSize);
    capulusPID.SetMode(AUTOMATIC);
}

void CAPULUS_PID::setCurrent(double temp){input = temp;}

void CAPULUS_PID::setTarget(double temp){setpoint = temp;}

bool CAPULUS_PID::signal(unsigned long now){
  if (windowStartTime == 0) windowStartTime = now;
  capulusPID.Compute();
  if (now - windowStartTime > windowSize) windowStartTime += windowSize;
  if (output > now - windowStartTime) return true;
  return false;
}

//TODO add autotune https://playground.arduino.cc/Code/PIDAutotuneLibrary/