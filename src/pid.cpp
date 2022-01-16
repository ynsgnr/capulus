#include "pid.h"
#include <PID_AutoTune_v0.h>
#include <Arduino.h>

double setpoint, input, output, windowSize;
unsigned long windowStartTime;
PID capulusPID(&input, &output, &setpoint, 2, 5, 1, DIRECT);

boolean tuning = false;
PID_ATune aTune(&input, &output);

CAPULUS_PID::CAPULUS_PID(double ws, double kp, double ki, double kd)
{
  capulusPID.SetTunings(kp, ki, kd);
  windowStartTime = millis();
  windowSize = ws;
  capulusPID.SetOutputLimits(0, windowSize);
  capulusPID.SetMode(AUTOMATIC);
}

void CAPULUS_PID::setCurrent(double temp) { input = temp; }

void CAPULUS_PID::setTarget(double temp) { setpoint = temp; }

bool CAPULUS_PID::signal()
{
  capulusPID.Compute();
  unsigned long now = millis();
  windowStartTime += ((now - windowStartTime) / windowSize) * windowSize;
  return output > (now - windowStartTime);
}

double CAPULUS_PID::analogSignal()
{
  capulusPID.Compute();
  return output;
}

bool CAPULUS_PID::autotune(double noise, double step, int lookBack, double startValue)
{
  if (!tuning)
  {
    tuning = true;
    output = startValue;
    aTune.SetNoiseBand(noise);
    aTune.SetOutputStep(step);
    aTune.SetLookbackSec(lookBack);
    aTune.SetControlType(1); //set to PID
  }
  if (aTune.Runtime() != 0)
  {
    tuning = false;
    //we're done, set the tuning parameters
    capulusPID.SetTunings(aTune.GetKp(), aTune.GetKi(), aTune.GetKd());
  }
  return tuning;
}

double CAPULUS_PID::getKp() { return capulusPID.GetKp(); }
double CAPULUS_PID::getKi() { return capulusPID.GetKi(); }
double CAPULUS_PID::getKd() { return capulusPID.GetKd(); }
void CAPULUS_PID::setTunings(double kp, double ki, double kd)
{
  capulusPID.SetTunings(kp, ki, kd);
}