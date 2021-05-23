#include "state.h"
#include <EEPROM.h>

#define TEMP_ADDR 0
#define PRESS_ADDR 1
#define PREINF_PRESS_ADDR 3
#define PREINF_TIMER_ADDR 4
#define BREW_TIMER_ADDR 5
#define STEAM_TEMP_ADDR 6
#define SLEEP_TIMER_ADDR 7
#define CRED_ADDR 8
#define CHECK_VAL 128
#define PRESSION 10

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
  sdata.pressure = double(EEPROM.read(PRESS_ADDR))/double(PRESSION);
  sdata.preinfusionPressure = double(EEPROM.read(PREINF_PRESS_ADDR))/double(PRESSION);
  sdata.preinfusionTimerSeconds = EEPROM.read(PREINF_TIMER_ADDR);
  sdata.brewTimerSeconds = EEPROM.read(BREW_TIMER_ADDR);
  sdata.steamTemp = EEPROM.read(STEAM_TEMP_ADDR);
  sdata.sleepTimerMinutes = EEPROM.read(SLEEP_TIMER_ADDR);
  EEPROM.end();
  sdata.selected = 0;
}

void CAPULUS_STATE::persist_save(){
  EEPROM.begin(512);
  EEPROM.write(TEMP_ADDR, sdata.temp);
  EEPROM.write(PRESS_ADDR, sdata.pressure*PRESSION);
  EEPROM.write(PREINF_PRESS_ADDR, sdata.preinfusionPressure*PRESSION);
  EEPROM.write(PREINF_TIMER_ADDR, sdata.preinfusionTimerSeconds);
  EEPROM.write(BREW_TIMER_ADDR, sdata.brewTimerSeconds);
  EEPROM.write(STEAM_TEMP_ADDR, sdata.steamTemp);
  EEPROM.write(SLEEP_TIMER_ADDR, sdata.sleepTimerMinutes);
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
    case SELECT_PREINF_PRESS:
        currentVal=sdata.preinfusionPressure;
        diff=0.1;
        break;
    case SELECT_PREINF_TIMER:
        currentVal=sdata.preinfusionTimerSeconds;
        break;
    case SELECT_BREW_TIMER:
        currentVal=sdata.brewTimerSeconds;
        break;
    case SELECT_STEAM_TEMP:
        currentVal=sdata.steamTemp;
        break;
    case SELECT_SLEEP_TIMER:
        currentVal=sdata.sleepTimerMinutes;
        break;
    }
    if(input.plus){
        currentVal+=multiplier;
        multiplier+=diff;
    } else if (input.minus){
        currentVal-=multiplier;
        multiplier+=diff;
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
    case SELECT_PREINF_PRESS:
        sdata.preinfusionPressure=currentVal;
        break;
    case SELECT_PREINF_TIMER:
        sdata.preinfusionTimerSeconds=currentVal;
        break;
    case SELECT_BREW_TIMER:
        sdata.brewTimerSeconds=currentVal;
        break;
    case SELECT_STEAM_TEMP:
        sdata.steamTemp=currentVal;
        break;
    case SELECT_SLEEP_TIMER:
        sdata.sleepTimerMinutes=currentVal;
        break;
    }
    if (sdata.temp>MAX_TEMP) sdata.temp=MAX_TEMP;
    if (sdata.pressure>MAX_PRESS) sdata.pressure=MAX_PRESS;
    if (sdata.preinfusionPressure>MAX_PRESS) sdata.preinfusionPressure=MAX_PRESS;
    if (sdata.preinfusionTimerSeconds>MAX_BREW_TIME) sdata.preinfusionTimerSeconds=MAX_BREW_TIME;
    if (sdata.brewTimerSeconds>MAX_BREW_TIME) sdata.temp=MAX_BREW_TIME;
    if (sdata.steamTemp>MAX_TEMP) sdata.steamTemp=MAX_TEMP;
    if (sdata.sleepTimerMinutes>MAX_SLEEP_TIME) sdata.pressure=MAX_SLEEP_TIME;
}

stateData CAPULUS_STATE::data(){return sdata;}