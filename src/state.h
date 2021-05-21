#ifndef CAPULUS_STATE_H
#define CAPULUS_STATE_H

#include <Arduino.h>
#include <input.h>

#define SELECT_TEMP 0
#define SELECT_PRESS 1
#define SELECT_BREW_TIMER 2
#define SELECT_SHUTDOWN_TIMER 3
#define INCREASE_SELECTED(selected) (selected+1)%4

struct stateData{
    char ssid[32] = "";
    char password[32] = "";
    int temp = 90;
    int pressure = 9;
    int shutdownTimerMinutes = 15;
    int brewTimerSeconds = 20;
    int selected;
};

class CAPULUS_STATE {
public:
    CAPULUS_STATE();
    void input(inputData);
    stateData data();
private:
    void persist_load();
    void persist_save();
    int multiplier = 1;
    stateData sdata;
};

#endif