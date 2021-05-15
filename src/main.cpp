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

// the loop function runs over and over again forever
void loop() {
  float currentTemp = read_temp();
  Serial.println(currentTemp,3);
  if(!digitalRead(BUTTON_1)){
    if (selected == SELECT_TEMP){
      selected = SELECT_PRESS;
    }else{
      selected = SELECT_TEMP;
    }
  } else if(!digitalRead(BUTTON_0)){
    if (selected == SELECT_TEMP){
      temp++;
    }else{
      pressure++;
    }
  } else if (!digitalRead(BUTTON_2)){
    if (selected == SELECT_TEMP){
      temp--;
    }else{
      pressure--;
    }
  }
  capulus_display(selected,temp,currentTemp,pressure);
  delay(300);
}