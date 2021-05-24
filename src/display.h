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
#define C_TEXT " C"
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
#define FINISHED_TEXT "finished"
#define TEMP_GRAPH_TEXT "temp graph"

class CAPULUS_DISPLAY{
    public:
        CAPULUS_DISPLAY();
        // state - displays menu and current settings
        void state(stateData,float);
        // sleep - displays sleep message
        void sleep();
        // realtime - displays status and temp graph over sample period, historic data is kept on OLED memory
        // this function expects setTemp and totalTime not to be changed during a realtime display session
        // in the case that they change screen need to be reset with sleep or state functions otherwise
        // graph will be broken
        void realtime(float currentTemp, int setTemp, double setpressure, String currentState, unsigned long remainingTime, unsigned long totalTime);
    private:
        unsigned long realtimeLastRefresh=0;
        int realtimeT=0;
};

#endif