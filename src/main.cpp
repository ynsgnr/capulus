#include <Arduino.h>
#include <temp.sensor.h>
#include <state.h>
#include <display.h>
#include <input.buttons.h>
#include <pid.h>
#include <timer.h>

#define HEATER_PIN 16 //D0 - blue
#define PUMP_PWM_PIN 14 //D5 - orange
#define READY_LED_PIN 0 //D3 - green

#define INPUT_INTERVAL 250
#define TEMP_INTERVAL 300
#define DISPLAY_INTERVAL 100
#define PUMP_PRESSURE 15
#define MAX_PWM 1023
#define TEMP_RANGE 1

CAPULUS_STATE state;
CAPULUS_BUTTON_INPUT buttons;
CAPULUS_DISPLAY display;
CAPULUS_PID pid(TEMP_INTERVAL);

TIMER sleepTimer;
TIMER brewTimer;
TIMER preinfusionTimer;

int inputLastRefresh;
int tempLastRefresh;
int displayLastRefresh;
float currentTemp;
bool brewing = false;
bool preinfusing = false;
bool sleep = false;
stateData data;
inputData buttonInput;

void setup(){
  state = CAPULUS_STATE();
  buttons = CAPULUS_BUTTON_INPUT();
  display = CAPULUS_DISPLAY();
  pid = CAPULUS_PID(TEMP_INTERVAL);
  sleepTimer.start();
  pinMode(HEATER_PIN,OUTPUT);
  pinMode(PUMP_PWM_PIN,OUTPUT);
  pinMode(READY_LED_PIN,OUTPUT);
}

void loop() {
  auto now = millis();
  if((now-inputLastRefresh)>INPUT_INTERVAL){
    inputLastRefresh+=INPUT_INTERVAL;
    buttonInput = buttons.read();
    if (buttonInput.any) sleepTimer.start();
    if (buttonInput.brew && !sleep){
      if (buttonInput.steam){
        analogWrite(PUMP_PWM_PIN,MAX_PWM);
      }else{
        if (!preinfusing) preinfusionTimer.start();
        preinfusing = true;
        if (data.preinfusionTimerSeconds>0 && !preinfusionTimer.timedOut()){
          analogWrite(PUMP_PWM_PIN,data.preinfusionPressure/PUMP_PRESSURE*MAX_PWM);
        }else{
          if (!brewing) brewTimer.start();
          brewing = true;
          if (brewTimer.timedOut()) analogWrite(PUMP_PWM_PIN,0);
          else analogWrite(PUMP_PWM_PIN,data.pressure/PUMP_PRESSURE*MAX_PWM);
        }
      }
    }else{
      preinfusing = false;
      brewing = false;
      analogWrite(PUMP_PWM_PIN,0);
      //only change state when not brewing
      state.input(buttonInput);
      data = state.data();
      if (buttonInput.steam) pid.setTarget(data.steamTemp);
      else pid.setTarget(data.temp);
      brewTimer.setTimeOut(data.brewTimerSeconds*SECOND);
      sleepTimer.setTimeOut(data.sleepTimerMinutes*MINUTE);
      preinfusionTimer.setTimeOut(data.preinfusionTimerSeconds*SECOND);
    }
  }
  if((now-tempLastRefresh)>TEMP_INTERVAL){
    tempLastRefresh+=TEMP_INTERVAL;
    currentTemp = read_temp();
    pid.setCurrent(double(currentTemp));
    sleep = sleepTimer.timedOut();
    if (pid.signal() && !sleep) digitalWrite(HEATER_PIN,HIGH);
    else digitalWrite(HEATER_PIN,LOW);
  }
  if((now-displayLastRefresh)>DISPLAY_INTERVAL){
    displayLastRefresh+=DISPLAY_INTERVAL;
    int targetTemp = data.temp;
    if (buttonInput.steam) targetTemp = data.steamTemp;
    if (currentTemp>=targetTemp-TEMP_RANGE && currentTemp<=targetTemp+TEMP_RANGE) digitalWrite(READY_LED_PIN,HIGH);
    else digitalWrite(READY_LED_PIN,LOW);
    unsigned long totalTime = (data.brewTimerSeconds + data.preinfusionTimerSeconds)*SECOND;
    if (sleep) display.sleep();
    else if (brewing) display.realtime(currentTemp, data.temp, data.pressure, String(BREWING_TEXT), brewTimer.remaining()/SECOND, totalTime);
    else if (preinfusing) display.realtime(currentTemp, data.temp, data.preinfusionPressure, String(PREINFING_TEXT), preinfusionTimer.remaining()/SECOND, totalTime);
    else display.state(data,currentTemp);
  }
}