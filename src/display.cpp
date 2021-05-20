#include "display.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 capulus_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TEXT_SIZE 2
#define LINE_SIZE 8
#define SECOND_LINE TEXT_SIZE*(LINE_SIZE*2)

#define TEMP_TEXT "temp"
#define PRESS_TEXT "pressure"
#define SEPERATOR "-"

CAPULUS_DISPLAY::CAPULUS_DISPLAY(){
    capulus_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void CAPULUS_DISPLAY::print(outputData data, float currentTemp ){
    capulus_display.clearDisplay();
    capulus_display.setTextSize(TEXT_SIZE); // Draw 2X-scale text
    capulus_display.setTextColor(SSD1306_WHITE);
    capulus_display.setCursor(0, 0);
    if (data.selected == SELECT_TEMP){
        capulus_display.print(F(">"));
    }
    capulus_display.println(F(TEMP_TEXT));
    capulus_display.print(String(currentTemp, 2));
    capulus_display.print(F(SEPERATOR));
    capulus_display.println(String(data.temp,10));

    capulus_display.setCursor(0, SECOND_LINE);
    if (data.selected == SELECT_PRESS){
        capulus_display.print(F(">"));
    }
    capulus_display.println(F(PRESS_TEXT));
    capulus_display.println(String(data.pressure,10));

    capulus_display.display();
}