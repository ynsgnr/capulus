#ifndef CAPULUS_PID_H
#define CAPULUS_PID_H

#include <PID_v1.h>

class CAPULUS_PID {
public:
    CAPULUS_PID();
    void setCurrent(double);
    void setTarget(double);
    bool signal(unsigned long);
private:
    double setpoint, input, output, windowSize;
    unsigned long windowStartTime;
    PID capulusPID;
};

#endif