#include <Arduino.h>
#include <persist.h>
#include <display.h>
#include <temp.sensor.h>
#include <input.h>

#define BUTTON_0 3 //RX
#define BUTTON_1 2 //D4
#define BUTTON_2 1 //TX

// the setup function runs once when you press reset or power the board
void setup() {
  get_persist();
  caplus_display_begin();
  input_begin();
}

int selected = SELECT_TEMP;
int multiplier = 1;

// the loop function runs over and over again forever
void loop() {
  float currentTemp = read_temp();
  inputData input = input_read(); 
  if(input.option){
    if (selected == SELECT_TEMP){
      selected = SELECT_PRESS;
    }else{
      selected = SELECT_TEMP;
    }
  }
  if(input.plus){
    multiplier++;
    if (selected == SELECT_TEMP){
      temp+=multiplier;
    }else{
      pressure+=multiplier;
    }
  } else if (input.minus){
    multiplier++;
    if (selected == SELECT_TEMP){
      temp-=multiplier;
    }else{
      pressure-=multiplier;
    }
  }else{
    multiplier = 1;
  }
  capulus_display(selected,temp,currentTemp,pressure);
  delay(100);
}