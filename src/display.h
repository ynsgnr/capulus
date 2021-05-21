#ifndef CAPULUS_DISPLAY_H
#define CAPULUS_DISPLAY_H

#include <state.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class CAPULUS_DISPLAY{
    public:
        CAPULUS_DISPLAY();
        void print(stateData,float,bool);
};

#endif