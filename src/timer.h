#ifndef CAPULUS_TIMER_H
#define CAPULUS_TIMER_H

#define SECOND 1000
#define MINUTE SECOND*60

class TIMER {
public:
    void start();
    unsigned long duration();
    unsigned long remaining();
    void stop();
    bool timedOut();
    void setTimeOut(unsigned long);
private:
    unsigned long startTime=0;
    unsigned long timeout=0;
    bool alreadyTimedOut=false;
};

#endif