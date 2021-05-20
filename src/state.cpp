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
  data.temp = EEPROM.read(TEMP_ADDR);
  data.pressure = EEPROM.read(PRESS_ADDR);
  EEPROM.get(CRED_ADDR, data.ssid);
  EEPROM.get(CRED_ADDR+sizeof(data.ssid), data.password);
  int check = EEPROM.read(CRED_ADDR+sizeof(data.ssid)+sizeof(data.password));
  EEPROM.end();
  if (check!=CHECK_VAL) {
    data.ssid[0] = 0;
    data.password[0] = 0;
  }
  data.selected = 0;
}

void CAPULUS_STATE::persist_save(){
  EEPROM.begin(512);
  EEPROM.write(TEMP_ADDR, data.temp);
  EEPROM.write(PRESS_ADDR, data.pressure);
  EEPROM.put(CRED_ADDR, data.ssid);
  EEPROM.put(CRED_ADDR+sizeof(data.ssid), data.password);
  EEPROM.write(CRED_ADDR+sizeof(data.ssid)+sizeof(data.password), CHECK_VAL);
  EEPROM.commit();
  EEPROM.end();
}

void CAPULUS_STATE::input(inputData input){
    if(input.option){
        data.selected = INCREASE_SELECTED(data.selected);
    }
    if(input.plus){
        multiplier++;
        if (selected == SELECT_TEMP){
        data.temp+=multiplier;
        }else{
        data.pressure+=multiplier;
        }
    } else if (input.minus){
        multiplier++;
        if (selected == SELECT_TEMP){
        data.temp-=multiplier;
        }else{
        data.pressure-=multiplier;
        }
    }else{
        if (multiplier!=1){
            persist_save();
        }
        multiplier = 1;
    }
}


outputData CAPULUS_STATE::output(){
    return data;
}