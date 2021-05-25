#ifndef CAPULUS_PID_H
#define CAPULUS_PID_H

#include <PID_v1.h>

class CAPULUS_PID {
public:
    CAPULUS_PID(double ws, double kp, double ki, double kd);
    void setCurrent(double);
    void setTarget(double);
    //signal - returns the signal for heater
    //depends on target and current 
    //set those values with setCurrent and setTarget
    //returns if the heater should run or not
    bool signal();
    //analogSignal - returns the analog signal for heater
    //can be used with PWM controlled heaters
    double analogSignal();
    //autotune - autotunes the paramaters for pid
    //signal should be called after each call to this function
    //returns if the autotune should be repeated or not
    //autotune is only completed when this returns false
    bool autotune(double noise, double step, int lookBack, double startValue);
    double getKp();
    double getKi();
    double getKd();
    void setTunings(double kp, double ki, double kd);
};

#endif