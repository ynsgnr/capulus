#ifndef CAPULUS_DISPLAY_H
#define CAPULUS_DISPLAY_H

#include <state.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define TEMP_TEXT "temp"
#define PRESS_TEXT "pressure"
#define PREINF_PRESS_TEXT "preinf press"
#define PREINF_TIMER_TEXT "preinf time"
#define BREW_TIMER_TEXT "brew time"
#define STEAM_TEMP_TEXT "steam temp"
#define BAR_TEXT " bar"
#define SECONDS_TEXT " s"
#define SLEEP_TIMER_TEXT "sleep timer"
#define MINUTES_TEXT " m"
#define DISABLED_TEXT "disabled"
#define TIMEOUT_TEXT "Sleep Mode"
#define RESTART_TEXT_1 "press a button"
#define RESTART_TEXT_2 "to wake up"
#define SEPERATOR "-"
#define PREINFING_TEXT "preinfusion"
#define BREWING_TEXT "brew"

class CAPULUS_DISPLAY{
    public:
        CAPULUS_DISPLAY();
        void printState(stateData,float);
        void printSleep();
        void printRealtime(stateData data, float currentTemp, String currentState, unsigned long  remainingTime);
};

#endif