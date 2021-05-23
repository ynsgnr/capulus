#include "timer.h"
#include <Arduino.h>

void TIMER::start(){
    startTime = millis();
    alreadyTimedOut = false;
}

void TIMER::stop(){
    startTime = 0;
    alreadyTimedOut = false;
}

void TIMER::setTimeOut(unsigned long to){timeout = to;}

unsigned long TIMER::remaining(){
    if (startTime==0) return 0;
    if (alreadyTimedOut) return 0;
    return timeout-duration();
}

unsigned long TIMER::duration(){
    if (startTime==0)return 0;
    return millis()-startTime;
}

bool TIMER::timedOut(){
    if (timeout==0)return false;
    alreadyTimedOut = duration()>=timeout;
    return alreadyTimedOut;
}