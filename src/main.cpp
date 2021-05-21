#include <Arduino.h>
#include <temp.sensor.h>
#include <state.h>
#include <display.h>
#include <input.buttons.h>
#include <pid.h>
#include <timer.h>

#define INPUT_INTERVAL 250
#define TEMP_INTERVAL 300
#define HEATER_PIN 16

CAPULUS_STATE state;
CAPULUS_BUTTON_INPUT buttons;
CAPULUS_DISPLAY display;
CAPULUS_PID pid;

TIMER shutdownTimer;
TIMER brewTimer;

int inputLastRefresh;
int tempLastRefresh;
float currentTemp;
stateData data;

void setup(){
  state = CAPULUS_STATE();
  buttons = CAPULUS_BUTTON_INPUT();
  display = CAPULUS_DISPLAY();
  pid = CAPULUS_PID();
  shutdownTimer.start();
  pinMode(HEATER_PIN,OUTPUT);
}

void loop() {
  auto now = millis();
  if(now-inputLastRefresh>INPUT_INTERVAL){
    inputLastRefresh+=INPUT_INTERVAL;
    inputData input = buttons.read();
    if (input.any) shutdownTimer.start();
    state.input(input);
    data = state.data();
    pid.setTarget(data.temp);
    brewTimer.setTimeOut(data.brewTimerSeconds*SECOND);
    shutdownTimer.setTimeOut(data.shutdownTimerMinutes*MINUTE);
  }
  if(now-tempLastRefresh>TEMP_INTERVAL){
    tempLastRefresh+=TEMP_INTERVAL;
    currentTemp = read_temp();
    bool sleep = shutdownTimer.timedOut();
    display.print(data,currentTemp,sleep);
    pid.setCurrent(double(currentTemp));
    if (pid.signal() && !sleep) digitalWrite(HEATER_PIN,HIGH);
    else digitalWrite(HEATER_PIN,LOW);
  }
}