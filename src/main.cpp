#include <Arduino.h>
#include <persist.h>
#include <display.h>
#include <temp.sensor.h>

#define BUTTON_0 2 //D4
#define BUTTON_1 3 //RX
#define BUTTON_2 16 //D0

bool buttonState = true;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  delay(100);
  Serial.println("\nSetting up");
  get_persist();
  caplus_display_begin();

  pinMode(BUTTON_0,INPUT);
  pinMode(BUTTON_1,INPUT);
  pinMode(BUTTON_2,INPUT);
}

int selected = SELECT_TEMP;
int multiplier = 1;

// the loop function runs over and over again forever
void loop() {
  float currentTemp = read_temp();
  if(!digitalRead(BUTTON_1)){
    if (selected == SELECT_TEMP){
      selected = SELECT_PRESS;
    }else{
      selected = SELECT_TEMP;
    }
  }
  if(!digitalRead(BUTTON_0)){
    multiplier++;
    if (selected == SELECT_TEMP){
      temp+=multiplier;
    }else{
      pressure+=multiplier;
    }
  } else if (!digitalRead(BUTTON_2)){
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
  delay(300);
}