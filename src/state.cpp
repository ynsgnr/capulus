#include "state.h"
#include <EEPROM.h>

#define TEMP_ADDR 0
#define PRESS_ADDR 1
#define BREW_TIMER_ADDR 2
#define SHUTDOWN_TIMER_ADDR 3
#define CRED_ADDR 4
#define CHECK_VAL 128

CAPULUS_STATE::CAPULUS_STATE(){
    persist_load();
}

void CAPULUS_STATE::persist_load(){
  EEPROM.begin(512);
  EEPROM.get(CRED_ADDR, sdata.ssid);
  EEPROM.get(CRED_ADDR+sizeof(sdata.ssid), sdata.password);
  int check = EEPROM.read(CRED_ADDR+sizeof(sdata.ssid)+sizeof(sdata.password));
  if (check!=CHECK_VAL) {
    sdata.ssid[0] = 0;
    sdata.password[0] = 0;
    return; //don't override default values if its first boot
  }
  sdata.temp = EEPROM.read(TEMP_ADDR);
  sdata.pressure = EEPROM.read(PRESS_ADDR);
  sdata.brewTimerSeconds = EEPROM.read(BREW_TIMER_ADDR);
  sdata.shutdownTimerMinutes = EEPROM.read(SHUTDOWN_TIMER_ADDR);
  EEPROM.end();
  sdata.selected = 0;
}

void CAPULUS_STATE::persist_save(){
  EEPROM.begin(512);
  EEPROM.write(TEMP_ADDR, sdata.temp);
  EEPROM.write(PRESS_ADDR, sdata.pressure);
  EEPROM.write(BREW_TIMER_ADDR, sdata.brewTimerSeconds);
  EEPROM.write(SHUTDOWN_TIMER_ADDR, sdata.shutdownTimerMinutes);
  EEPROM.put(CRED_ADDR, sdata.ssid);
  EEPROM.put(CRED_ADDR+sizeof(sdata.ssid), sdata.password);
  EEPROM.write(CRED_ADDR+sizeof(sdata.ssid)+sizeof(sdata.password), CHECK_VAL);
  EEPROM.commit();
  EEPROM.end();
}

void CAPULUS_STATE::input(inputData input){
    if(input.option){
        sdata.selected = INCREASE_SELECTED(sdata.selected);
        return;
    }
    double currentVal=0;
    double diff = 1;
    switch (sdata.selected){
    case SELECT_TEMP:
        currentVal=sdata.temp;
        break;
    case SELECT_PRESS:
        currentVal=sdata.pressure;
        diff=0.1;
        break;
    case SELECT_BREW_TIMER:
        currentVal=sdata.brewTimerSeconds;
        break;
    case SELECT_SHUTDOWN_TIMER:
        currentVal=sdata.shutdownTimerMinutes;
        break;
    }
    if(input.plus){
        multiplier+=diff;
        currentVal+=multiplier;
    } else if (input.minus){
        multiplier+=diff;
        currentVal-=multiplier;
        if (currentVal<=0) currentVal = 0;
    }else{
        if (multiplier!=diff) persist_save();
        multiplier = diff;
    }
    switch (sdata.selected){
    case SELECT_TEMP:
        sdata.temp=currentVal;
        break;
    case SELECT_PRESS:
        sdata.pressure=currentVal;
        break;
    case SELECT_BREW_TIMER:
        sdata.brewTimerSeconds=currentVal;
        break;
    case SELECT_SHUTDOWN_TIMER:
        sdata.shutdownTimerMinutes=currentVal;
        break;
    }
    if (sdata.temp>MAX_TEMP)sdata.temp=MAX_TEMP;
    if (sdata.pressure>MAX_PRESS)sdata.pressure=MAX_PRESS;
}

stateData CAPULUS_STATE::data(){return sdata;}