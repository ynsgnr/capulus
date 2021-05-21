#include "display.h"
#include "state.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 capulus_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TEXT_SIZE 2

#define TEMP_TEXT "temp"
#define PRESS_TEXT "pressure"
#define TIMEOUT_TEXT "Sleep Mode"
#define RESTART_TEXT "press a button to wake up"
#define SEPERATOR "-"

CAPULUS_DISPLAY::CAPULUS_DISPLAY(){
    capulus_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void CAPULUS_DISPLAY::print(stateData data, float currentTemp, bool timedOut){
    capulus_display.clearDisplay();
        capulus_display.setTextSize(TEXT_SIZE);
    if (timedOut){
        capulus_display.println(F(TIMEOUT_TEXT));
        capulus_display.println(F(RESTART_TEXT));
        return;
    }
    else capulus_display.setTextSize(TEXT_SIZE);
    capulus_display.setTextColor(SSD1306_WHITE);
    capulus_display.setCursor(0, 0);
    if (data.selected == SELECT_TEMP){
        capulus_display.print(F(">"));
    }
    capulus_display.println(F(TEMP_TEXT));
    capulus_display.print(String(currentTemp, 2));
    capulus_display.print(F(SEPERATOR));
    capulus_display.println(String(data.temp,10));
    if (data.selected == SELECT_PRESS){
        capulus_display.print(F(">"));
    }
    capulus_display.println(F(PRESS_TEXT));
    capulus_display.println(String(data.pressure,1));
    capulus_display.display();
}