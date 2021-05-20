#include <Arduino.h>
#include <temp.sensor.h>
#include <state.h>
#include <display.h>

CAPULUS_STATE state;
CAPULUS_INPUT in;
CAPULUS_DISPLAY display;

void setup(){
  state = CAPULUS_STATE();
  in = CAPULUS_INPUT();
  display = CAPULUS_DISPLAY();
}

void loop() {
  float currentTemp = read_temp();
  state.input(in.read());
  display.print(state.output(),currentTemp);
  delay(100);
}