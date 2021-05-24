#include "display.h"
#include "state.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

#define TEXT_SIZE 2
#define LINE_SIZE 8
#define PAGE_COUNT 4
#define PAGE_INDICATOR_SIZE 4
#define PAGE_INDICATOR_PADDING 1
#define INDICATOR_HEIGHT 62 /* SCREEN_HEIGHT-PAGE_INDICATOR_PADDING*2 */
#define SECTION_HEIGHT 15 /* (INDICATOR_HEIGHT)/(PAGE_COUNT) */

#define REALTIME_MIN_TEMP 67
#define REALTIME_MAX_TEMP 113
#define REALTIME_TEXT_SIZE 1 /* TEXT_SIZE/2 */
#define REALTIME_GRAPH_HEIGHT 48 /* SCREEN_HEIGHT-REALTIME_TEXT_SIZE*LINE_SIZE*2 */
#define REALTIME_STATUS_BAR_HEIGHT 16 /* SCREEN_HEIGHT-REALTIME_GRAPH_HEIGHT */
#define REALTIME_STATUS_BAR_Y REALTIME_GRAPH_HEIGHT
#define REALTIME_UNIT_TEMP 1 /* (REALTIME_MAX_TEMP-REALTIME_MIN_TEMP)/REALTIME_GRAPH_HEIGHT */

Adafruit_SSD1306 capulus_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

CAPULUS_DISPLAY::CAPULUS_DISPLAY(){
    capulus_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void CAPULUS_DISPLAY::realtime(float currentTemp, int setTemp, double setpressure, String currentState, unsigned long remainingTime){
    if (realtimeDrawing){
        //only refresh status bar since graph data is only kept on OLED memory
        capulus_display.fillRect(0, REALTIME_STATUS_BAR_Y, SCREEN_WIDTH, REALTIME_STATUS_BAR_HEIGHT, SSD1306_BLACK);
    }else{
        capulus_display.clearDisplay();
        realtimeDrawing = true;
    }

    //draw set temp line and set temp
    int y = REALTIME_GRAPH_HEIGHT-REALTIME_UNIT_TEMP*(setTemp-REALTIME_MIN_TEMP);
    int currentTempSizeX = SCREEN_WIDTH-LINE_SIZE*3;
    capulus_display.drawLine(0, y, currentTempSizeX, y, SSD1306_WHITE);
    capulus_display.setTextSize(REALTIME_TEXT_SIZE);
    capulus_display.setTextColor(SSD1306_WHITE);
    capulus_display.setCursor(currentTempSizeX, y-LINE_SIZE/2);

    //draw status bar
    capulus_display.print(String(setTemp,10));
    capulus_display.setCursor(0, REALTIME_STATUS_BAR_Y);
    capulus_display.println(currentState);
    capulus_display.print(String(currentTemp, 2));
    capulus_display.print(F(C_TEXT));
    capulus_display.print(F(SEPERATOR));
    capulus_display.print(String(setpressure, 2));
    capulus_display.print(F(BAR_TEXT));
    capulus_display.print(F(SEPERATOR));
    capulus_display.print(String(remainingTime, 10));
    capulus_display.println(F(SECONDS_TEXT));

    capulus_display.display();
}

void CAPULUS_DISPLAY::sleep(){
    realtimeDrawing = false;
    capulus_display.clearDisplay();
    capulus_display.setTextSize(TEXT_SIZE);
    capulus_display.setTextColor(SSD1306_WHITE);
    capulus_display.setCursor(0, 0);
    capulus_display.println(F(TIMEOUT_TEXT));
    capulus_display.setTextSize(TEXT_SIZE/2);
    capulus_display.println(F(""));
    capulus_display.println(F(""));
    capulus_display.println(F(RESTART_TEXT_1));
    capulus_display.println(F(RESTART_TEXT_2));
    capulus_display.display();
}

void CAPULUS_DISPLAY::state(stateData data, float currentTemp){
    realtimeDrawing = false;
    capulus_display.clearDisplay();
    capulus_display.setTextSize(TEXT_SIZE);
    capulus_display.setTextColor(SSD1306_WHITE);
    capulus_display.setCursor(0, 0);
    int page;
    switch (data.selected){
    case SELECT_TEMP:
    case SELECT_PRESS:
        page=0;
        if (data.selected == SELECT_TEMP) capulus_display.print(F(">"));
        capulus_display.println(F(TEMP_TEXT));
        capulus_display.print(String(currentTemp, 2));
        capulus_display.print(F(SEPERATOR));
        capulus_display.println(String(data.temp,10));
        if (data.selected == SELECT_PRESS) capulus_display.print(F(">"));
        capulus_display.println(F(PRESS_TEXT));
        capulus_display.print(String(data.pressure,1));
        capulus_display.println(F(BAR_TEXT));
        break;
    case SELECT_PREINF_PRESS:
    case SELECT_PREINF_TIMER:
        page=1;
        if (data.selected == SELECT_PREINF_PRESS) capulus_display.print(F(">"));
        capulus_display.setTextSize(TEXT_SIZE/2,TEXT_SIZE);
        capulus_display.println(F(PREINF_PRESS_TEXT));
        capulus_display.setTextSize(TEXT_SIZE);
        capulus_display.print(String(data.preinfusionPressure,1));
        capulus_display.println(F(BAR_TEXT));
        if (data.selected == SELECT_PREINF_TIMER) capulus_display.print(F(">"));
        capulus_display.setTextSize(TEXT_SIZE/2,TEXT_SIZE);
        capulus_display.println(F(PREINF_TIMER_TEXT));
        capulus_display.setTextSize(TEXT_SIZE);
        if (data.preinfusionTimerSeconds==0){
            capulus_display.println(F(DISABLED_TEXT));
        }else{
            capulus_display.print(String(data.preinfusionTimerSeconds,10));
            capulus_display.println(F(SECONDS_TEXT));
        }
        break;
    case SELECT_BREW_TIMER:
    case SELECT_STEAM_TEMP:
        page=2;
        if (data.selected == SELECT_BREW_TIMER) capulus_display.print(F(">"));
        capulus_display.setTextSize(TEXT_SIZE/2,TEXT_SIZE);
        capulus_display.println(F(BREW_TIMER_TEXT));
        capulus_display.setTextSize(TEXT_SIZE);
        capulus_display.print(String(data.brewTimerSeconds,10));
        capulus_display.println(F(SECONDS_TEXT));
        if (data.selected == SELECT_STEAM_TEMP) capulus_display.print(F(">"));
        capulus_display.setTextSize(TEXT_SIZE/2,TEXT_SIZE);
        capulus_display.println(F(STEAM_TEMP_TEXT));
        capulus_display.setTextSize(TEXT_SIZE);
        capulus_display.print(String(currentTemp, 2));
        capulus_display.print(F(SEPERATOR));
        capulus_display.println(String(data.steamTemp,10));
        break;
    case SELECT_SLEEP_TIMER:
        page=3;
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
        break;
    default:
        page = -1;
        break;
    }
    //draw page indicator
    capulus_display.drawRoundRect(
        SCREEN_WIDTH-PAGE_INDICATOR_SIZE-PAGE_INDICATOR_PADDING,
        PAGE_INDICATOR_PADDING,
        PAGE_INDICATOR_SIZE,
        INDICATOR_HEIGHT,
        PAGE_INDICATOR_SIZE,
        SSD1306_WHITE);
    capulus_display.fillRoundRect(
        SCREEN_WIDTH-PAGE_INDICATOR_SIZE-PAGE_INDICATOR_PADDING,
        PAGE_INDICATOR_PADDING+SECTION_HEIGHT*page,
        PAGE_INDICATOR_SIZE,
        SECTION_HEIGHT,
        PAGE_INDICATOR_SIZE,
        SSD1306_WHITE);
    capulus_display.display();
}