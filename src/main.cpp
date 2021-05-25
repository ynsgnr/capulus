#include <Arduino.h>
#include <temp.sensor.h>
#include <state.h>
#include <display.h>
#include <input.buttons.h>
#include <pid.h>
#include <timer.h>
#include <RBDdimmer.h>

#define HEATER_PIN 2 //D4 - yellow
#define PUMP_PWM_PIN 14 //D5 - orange
#define PUMP_ZX_PIN 16 //D0 - blue
#define READY_LED_PIN 0 //D3 - green

#define INPUT_INTERVAL 250
#define TEMP_INTERVAL 300
#define DISPLAY_INTERVAL 100
#define PUMP_PRESSURE 15
#define TEMP_RANGE 1

//pid paramaters
#define PID_KP 2
#define PID_KI 5
#define PID_KD 1
//autotune related stuff
#define ATUNE_STEP 5
#define ATUNE_NOISE 0.5
#define ATUNE_START 80
#define ATUNE_LOOKBACK 2

CAPULUS_STATE state;
CAPULUS_BUTTON_INPUT buttons;
CAPULUS_DISPLAY display;
CAPULUS_PID pid(TEMP_INTERVAL,PID_KP,PID_KI,PID_KD);
dimmerLamp pump(PUMP_PWM_PIN,PUMP_ZX_PIN);

TIMER sleepTimer;
TIMER brewTimer;
TIMER preinfusionTimer;

unsigned long inputLastRefresh;
unsigned long tempLastRefresh;
unsigned long displayLastRefresh;
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
  pid = CAPULUS_PID(TEMP_INTERVAL,PID_KP,PID_KI,PID_KD);
  sleepTimer.start();
  pinMode(HEATER_PIN,OUTPUT);
  pinMode(PUMP_PWM_PIN,OUTPUT);
  pinMode(READY_LED_PIN,OUTPUT);
  pump.begin(NORMAL_MODE, OFF);
}

void loop() {
  auto now = millis();
  if (data.autotuning){
    if((now-tempLastRefresh)>TEMP_INTERVAL){
      tempLastRefresh+=TEMP_INTERVAL;

      bool finished = false;
      buttonInput = buttons.read();
      if (!buttonInput.any) {
        display.autotune();
        pump.setPower(map(data.pressure, 0, PUMP_PRESSURE, 100, 0));
        currentTemp = read_temp();
        pid.setCurrent(double(currentTemp));
        if (pid.autotune(ATUNE_NOISE, ATUNE_STEP, ATUNE_LOOKBACK, ATUNE_START)){
          if (pid.signal()) digitalWrite(HEATER_PIN,HIGH);
          else digitalWrite(HEATER_PIN,LOW);
        }else{
          finished = true;
        }
      }else{
        finished = true;
      }

      if (finished){
        state.setTunings(pid.getKp(),pid.getKi(),pid.getKd());
        data = state.data();
        delay(1000);
        now = millis();
        inputLastRefresh = now;
        displayLastRefresh = now;
        tempLastRefresh = now;
      }
    }
    return;
  }
  if((now-inputLastRefresh)>INPUT_INTERVAL){
    inputLastRefresh+=INPUT_INTERVAL;

    buttonInput = buttons.read();
    if (buttonInput.any) sleepTimer.start();

    if (buttonInput.brew && !sleep){
      if (buttonInput.steam){
        pump.setPower(map(data.pressure, 0, PUMP_PRESSURE, 100, 0));
      }else{
        if (!preinfusing) preinfusionTimer.start();
        preinfusing = true;
        if (data.preinfusionTimerSeconds>0 && !preinfusionTimer.timedOut()){
        pump.setPower(map(data.preinfusionPressure, 0, PUMP_PRESSURE, 100, 0));
        }else{
          if (!brewing) brewTimer.start();
          brewing = true;
          if (brewTimer.timedOut()) pump.setPower(0);
          else pump.setPower(map(data.pressure, 0, PUMP_PRESSURE, 100, 0));
        }
      }
    }else{
      preinfusing = false;
      brewing = false;
      pump.setPower(0);
      //only change state when not brewing
      state.input(buttonInput);
      data = state.data();
      pid.setTunings(data.kp, data.ki, data.kd);
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
    else digitalWrite(HEATER_PIN,LOW) ;
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