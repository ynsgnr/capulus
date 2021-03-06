#include <Arduino.h>
#include <temp.sensor.h>
#include <state.h>
#include <display.h>
#include <input.buttons.h>
#include <pid.h>
#include <timer.h>
#include <RBDdimmer.h>

#define HEATER_PIN 15   //D8 - yellow
#define PUMP_PSM_PIN 14 //D5 - orange
#define PUMP_ZC_PIN 13  //D7 - blue
#define MIN_PUMP_PWR 60
#define MAX_PUMP_PWR 100
#define READY_LED_PIN 0 //D3 - green
#define READY_LED_BRIGHTNESS 341

#define INPUT_INTERVAL 250
#define TEMP_INTERVAL 300
#define DISPLAY_INTERVAL 100
#define PID_INTERVAL 400
#define PUMP_PRESSURE 15
#define TEMP_RANGE 3

//pid paramaters
#define PID_KP 8
#define PID_KI 1
#define PID_KD 2
//autotune related stuff
#define ATUNE_STEP 10
#define ATUNE_NOISE TEMP_RANGE
#define ATUNE_START 80
#define ATUNE_LOOKBACK 10

CAPULUS_STATE state;
CAPULUS_BUTTON_INPUT buttons;
CAPULUS_DISPLAY display;
CAPULUS_PID pid(TEMP_INTERVAL, PID_KP, PID_KI, PID_KD);
dimmerLamp pump(PUMP_PSM_PIN, PUMP_ZC_PIN);

TIMER sleepTimer;
TIMER brewTimer;
TIMER preinfusionTimer;

unsigned long inputLastRefresh;
unsigned long tempLastRefresh;
unsigned long displayLastRefresh;
unsigned long pidLastRefresh;
float currentTemp;
int targetTemp;
bool brewing = false;
bool preinfusing = false;
bool sleep = false;
stateData data;
inputData buttonInput;

void setup()
{
  state = CAPULUS_STATE();
  buttons = CAPULUS_BUTTON_INPUT();
  display = CAPULUS_DISPLAY();
  pid = CAPULUS_PID(TEMP_INTERVAL, PID_KP, PID_KI, PID_KD);
  sleepTimer.start();
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(READY_LED_PIN, OUTPUT);
  pump.begin(NORMAL_MODE, OFF);
}

void loop()
{
  auto now = millis();
  sleep = sleepTimer.timedOut();
  if ((now - inputLastRefresh) > INPUT_INTERVAL)
  {
    inputLastRefresh += INPUT_INTERVAL;

    buttonInput = buttons.read();
    if (buttonInput.any)
      sleepTimer.start();
    targetTemp = data.temp;
    if (buttonInput.steam)
      targetTemp = data.steamTemp;

    if (buttonInput.brew && !sleep)
    {
      if (buttonInput.steam)
      {
        pump.setPower(map(PUMP_PRESSURE, 0, PUMP_PRESSURE, MIN_PUMP_PWR, MAX_PUMP_PWR));
        pump.setState(ON);
      }
      else
      {
        if (!preinfusing)
          preinfusionTimer.start();
        preinfusing = true;
        if (data.preinfusionTimerSeconds > 0 && !preinfusionTimer.timedOut())
        {
          pump.setPower(map(data.preinfusionPressure, 0, PUMP_PRESSURE, MIN_PUMP_PWR, MAX_PUMP_PWR));
          pump.setState(ON);
        }
        else
        {
          if (!brewing)
            brewTimer.start();
          brewing = true;
          if (brewTimer.timedOut())
          {
            pump.setState(OFF);
          }
          else
          {
            pump.setPower(map(data.pressure, 0, PUMP_PRESSURE, MIN_PUMP_PWR, MAX_PUMP_PWR));
            pump.setState(ON);
          }
        }
      }
    }
    else
    {
      preinfusing = false;
      brewing = false;
      pump.setState(OFF);
      //only change state when not brewing
      state.input(buttonInput);
      data = state.data();
      pid.setTunings(PID_KP, PID_KI, PID_KD);
      if (buttonInput.steam)
        pid.setTarget(data.steamTemp);
      else
        pid.setTarget(data.temp);
      brewTimer.setTimeOut(data.brewTimerSeconds * SECOND);
      sleepTimer.setTimeOut(data.sleepTimerMinutes * MINUTE);
      preinfusionTimer.setTimeOut(data.preinfusionTimerSeconds * SECOND);
    }
  }
  if ((now - tempLastRefresh) > TEMP_INTERVAL)
  {
    tempLastRefresh += TEMP_INTERVAL;

    currentTemp = read_temp();
    pid.setCurrent(double(currentTemp));
  }
  if ((now - pidLastRefresh) > PID_INTERVAL)
  {
    pidLastRefresh += PID_INTERVAL;

    if (pid.signal() && !sleep)
      digitalWrite(HEATER_PIN, HIGH);
    else
      digitalWrite(HEATER_PIN, LOW);
  }
  if ((now - displayLastRefresh) > DISPLAY_INTERVAL)
  {
    displayLastRefresh += DISPLAY_INTERVAL;

    if (currentTemp >= targetTemp - TEMP_RANGE && currentTemp <= targetTemp + TEMP_RANGE)
      digitalWrite(READY_LED_PIN, HIGH);
    else
      digitalWrite(READY_LED_PIN, LOW);

    unsigned long totalTime = (data.brewTimerSeconds + data.preinfusionTimerSeconds) * SECOND;
    if (sleep)
      display.sleep();
    else if (brewing)
      display.realtime(currentTemp, data.temp, data.pressure, String(BREWING_TEXT), brewTimer.remaining() / SECOND, totalTime);
    else if (preinfusing)
      display.realtime(currentTemp, data.temp, data.preinfusionPressure, String(PREINFING_TEXT), preinfusionTimer.remaining() / SECOND, totalTime);
    else
      display.state(data, currentTemp);
  }
}