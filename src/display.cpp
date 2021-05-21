#include "display.h"
#include "state.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

#define TEXT_SIZE 2
#define PAGE_COUNT 2
#define PAGE_INDICATOR_SIZE 4
#define PAGE_INDICATOR_PADDING 1
#define INDICATOR_HEIGHT SCREEN_HEIGHT-PAGE_INDICATOR_PADDING*2
#define SECTION_HEIGHT (INDICATOR_HEIGHT)/(PAGE_COUNT)

#define TEMP_TEXT "temp"
#define PRESS_TEXT "pressure"
#define BREW_TIMER_TEXT "brew time"
#define SECONDS_TEXT " s"
#define SLEEP_TIMER_TEXT "sleep timer"
#define MINUTES_TEXT " m"
#define DISABLED_TEXT "disabled"
#define TIMEOUT_TEXT "Sleep Mode"
#define RESTART_TEXT_1 "press a button"
#define RESTART_TEXT_2 "to wake up"
#define SEPERATOR "-"

Adafruit_SSD1306 capulus_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

CAPULUS_DISPLAY::CAPULUS_DISPLAY(){
    capulus_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void CAPULUS_DISPLAY::print(stateData data, float currentTemp, bool timedOut){
    capulus_display.clearDisplay();
    capulus_display.setTextSize(TEXT_SIZE);
    capulus_display.setTextColor(SSD1306_WHITE);
    capulus_display.setCursor(0, 0);
    if (timedOut){
        capulus_display.println(F(TIMEOUT_TEXT));
        capulus_display.setTextSize(TEXT_SIZE/2);
        capulus_display.println(F(""));
        capulus_display.println(F(""));
        capulus_display.println(F(RESTART_TEXT_1));
        capulus_display.println(F(RESTART_TEXT_2));
        capulus_display.display();
        return;
    }
    int page=0;
    switch (data.selected){
    case SELECT_TEMP:
    case SELECT_PRESS:
        page=1;
        if (data.selected == SELECT_TEMP) capulus_display.print(F(">"));
        capulus_display.println(F(TEMP_TEXT));
        capulus_display.print(String(currentTemp, 2));
        capulus_display.print(F(SEPERATOR));
        capulus_display.println(String(data.temp,10));
        if (data.selected == SELECT_PRESS) capulus_display.print(F(">"));
        capulus_display.println(F(PRESS_TEXT));
        capulus_display.println(String(data.pressure,1));
        break;
    case SELECT_BREW_TIMER:
    case SELECT_SLEEP_TIMER:
        page=2;
        if (data.selected == SELECT_BREW_TIMER) capulus_display.print(F(">"));
        capulus_display.println(F(BREW_TIMER_TEXT));
        capulus_display.print(String(data.brewTimerSeconds,10));
        capulus_display.println(F(SECONDS_TEXT));
        if (data.selected == SELECT_SLEEP_TIMER) capulus_display.print(F(">"));
        capulus_display.setTextSize(TEXT_SIZE/2,TEXT_SIZE);
        capulus_display.println(F(SLEEP_TIMER_TEXT));
        capulus_display.setTextSize(TEXT_SIZE);
        if (data.sleepTimerMinutes==0){
            capulus_display.println(F(DISABLED_TEXT));
        }else{
            capulus_display.print(String(data.sleepTimerMinutes,10));
            capulus_display.println(F(MINUTES_TEXT));
        }
    default:
        break;
    }
    //draw page indicator
    if (page!=0){
        capulus_display.drawRoundRect(
            SCREEN_WIDTH-PAGE_INDICATOR_SIZE-PAGE_INDICATOR_PADDING,
            PAGE_INDICATOR_PADDING,
            PAGE_INDICATOR_SIZE,
            INDICATOR_HEIGHT,
            PAGE_INDICATOR_SIZE,
            SSD1306_WHITE);
        capulus_display.fillRoundRect(
            SCREEN_WIDTH-PAGE_INDICATOR_SIZE-PAGE_INDICATOR_PADDING,
            PAGE_INDICATOR_PADDING+SECTION_HEIGHT*(page-1),
            PAGE_INDICATOR_SIZE,
            SECTION_HEIGHT,
            PAGE_INDICATOR_SIZE,
            SSD1306_WHITE);
    }
    capulus_display.display();
}