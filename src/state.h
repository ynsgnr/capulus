#ifndef CAPULUS_STATE_H
#define CAPULUS_STATE_H

#include <Arduino.h>
#include <input.h>

#define SELECT_TEMP 0
#define SELECT_PRESS 1
#define SELECT_PREINF_PRESS 2
#define SELECT_PREINF_TIMER 3
#define SELECT_BREW_TIMER 4
#define SELECT_SLEEP_TIMER 5
#define INCREASE_SELECTED(selected) (selected+1)%6

#define MAX_TEMP 110
#define MAX_PRESS 15
#define MAX_BREW_TIME 60 //seconds
#define MAX_SLEEP_TIME 30 //minutes

struct stateData{
    char ssid[32] = "";
    char password[32] = "";
    int temp = 90;
    int steamTemp = 150;
    double pressure = 9;
    int preinfusionPressure = 3;
    int preinfusionTimerSeconds = 0;
    int sleepTimerMinutes = 5;
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
    double multiplier = 1;
    stateData sdata;
};

#endif