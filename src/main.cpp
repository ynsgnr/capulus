#include <Arduino.h>
#include <temp.sensor.h>
#include <state.h>
#include <display.h>
#include <input.buttons.h>
#include <pid.h>
#include <timer.h>

#define HEATER_PIN 16 //D0
#define PUMP_PWM_PIN 14 //D5
#define READY_LED_PIN 0 //D3

#define INPUT_INTERVAL 250
#define TEMP_INTERVAL 300
#define PUMP_PRESSURE 15
#define MAX_PWM 1023
#define TEMP_RANGE 2

CAPULUS_STATE state;
CAPULUS_BUTTON_INPUT buttons;
CAPULUS_DISPLAY display;
CAPULUS_PID pid;

TIMER sleepTimer;
TIMER brewTimer;
TIMER preinfusionTimer;

int inputLastRefresh;
int tempLastRefresh;
float currentTemp;
bool brewing = false;
bool preinfusing = false;
stateData data;

void setup(){
  state = CAPULUS_STATE();
  buttons = CAPULUS_BUTTON_INPUT();
  display = CAPULUS_DISPLAY();
  pid = CAPULUS_PID();
  sleepTimer.start();
  pinMode(HEATER_PIN,OUTPUT);
  pinMode(PUMP_PWM_PIN,OUTPUT);
  pinMode(READY_LED_PIN,OUTPUT);
}

void loop() {
  auto now = millis();
  if(now-inputLastRefresh>INPUT_INTERVAL){
    inputLastRefresh+=INPUT_INTERVAL;
    inputData input = buttons.read();
    if (input.any) sleepTimer.start();
    if (input.brew){
      if (input.steam){
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
      state.input(input);
      data = state.data();
      if (input.steam) pid.setTarget(data.steamTemp);
      else pid.setTarget(data.temp);
      brewTimer.setTimeOut(data.brewTimerSeconds*SECOND);
      sleepTimer.setTimeOut(data.sleepTimerMinutes*MINUTE);
      preinfusionTimer.setTimeOut(data.preinfusionTimerSeconds*SECOND);
    }
  }
  if(now-tempLastRefresh>TEMP_INTERVAL){
    tempLastRefresh+=TEMP_INTERVAL;
    currentTemp = read_temp();
    if (currentTemp>=data.temp-TEMP_RANGE && currentTemp<=data.temp+TEMP_RANGE) digitalWrite(READY_LED_PIN,HIGH);
    else digitalWrite(READY_LED_PIN,LOW);
    bool sleep = sleepTimer.timedOut();
    display.print(data,currentTemp,sleep);
    pid.setCurrent(double(currentTemp));
    if (pid.signal() && !sleep) digitalWrite(HEATER_PIN,HIGH);
    else digitalWrite(HEATER_PIN,LOW);
  }
}