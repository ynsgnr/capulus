#include "timer.h"
#include <Arduino.h>

void TIMER::start(){startTime = millis();}

void TIMER::stop(){startTime = 0;}

void TIMER::setTimeOut(unsigned long to){timeout = to;}

unsigned long TIMER::duration(){
    if (startTime==0)return 0;
    return millis()-startTime;
}

bool TIMER::timedOut(){
    if (timeout==0)return false;
    return duration()>=timeout;
}