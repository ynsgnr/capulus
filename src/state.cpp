#include "state.h"
#include <EEPROM.h>

#define TEMP_ADDR 0
#define PRESS_ADDR 1
#define CRED_ADDR 2
#define CHECK_VAL 128

CAPULUS_STATE::CAPULUS_STATE(){
    persist_load();
}

void CAPULUS_STATE::persist_load(){
  EEPROM.begin(512);
  sdata.temp = EEPROM.read(TEMP_ADDR);
  sdata.pressure = EEPROM.read(PRESS_ADDR);
  EEPROM.get(CRED_ADDR, sdata.ssid);
  EEPROM.get(CRED_ADDR+sizeof(sdata.ssid), sdata.password);
  int check = EEPROM.read(CRED_ADDR+sizeof(sdata.ssid)+sizeof(sdata.password));
  EEPROM.end();
  if (check!=CHECK_VAL) {
    sdata.ssid[0] = 0;
    sdata.password[0] = 0;
  }
  sdata.selected = 0;
}

void CAPULUS_STATE::persist_save(){
  EEPROM.begin(512);
  EEPROM.write(TEMP_ADDR, sdata.temp);
  EEPROM.write(PRESS_ADDR, sdata.pressure);
  EEPROM.put(CRED_ADDR, sdata.ssid);
  EEPROM.put(CRED_ADDR+sizeof(sdata.ssid), sdata.password);
  EEPROM.write(CRED_ADDR+sizeof(sdata.ssid)+sizeof(sdata.password), CHECK_VAL);
  EEPROM.commit();
  EEPROM.end();
}

void CAPULUS_STATE::input(inputData input){
    if(input.option){
        sdata.selected = INCREASE_SELECTED(sdata.selected);
    }
    if(input.plus){
        multiplier++;
        if (sdata.selected == SELECT_TEMP){
        sdata.temp+=multiplier;
        }else{
        sdata.pressure+=multiplier;
        }
    } else if (input.minus){
        multiplier++;
        if (sdata.selected == SELECT_TEMP){
        sdata.temp-=multiplier;
        }else{
        sdata.pressure-=multiplier;
        }
    }else{
        if (multiplier!=1){
            persist_save();
        }
        multiplier = 1;
    }
}


stateData CAPULUS_STATE::data(){
    return sdata;
}