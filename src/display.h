#ifndef CAPULUS_DISPLAY_H
#define CAPULUS_DISPLAY_H

#include "output.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class CAPULUS_DISPLAY{
    public:
        CAPULUS_DISPLAY();
        void print(outputData,float);
    private:
        Adafruit_SSD1306 display;
};

#endif