#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SELECT_TEMP 0
#define SELECT_PRESS 1

#define TEXT_SIZE 2
#define LINE_SIZE 8
#define SECOND_LINE TEXT_SIZE*(LINE_SIZE*2)

#define TEMP_TEXT "tempature"
#define PRESS_TEXT "pressure"
#define SEPERATOR "-"

void caplus_display_begin(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void capulus_display(int selected,int temp,float currentTemp,int pressure){
    display.clearDisplay();
    display.setTextSize(TEXT_SIZE); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    if (selected == SELECT_TEMP){
        display.print(F(">"));
    }
    display.println(F(TEMP_TEXT));
    display.print(String(currentTemp, 2));
    display.print(F(SEPERATOR));
    display.println(String(temp,10));

    display.setCursor(0, SECOND_LINE);
    if (selected == SELECT_PRESS){
        display.print(F(">"));
    }
    display.println(F(PRESS_TEXT));
    display.println(String(pressure,10));

    display.display();
}