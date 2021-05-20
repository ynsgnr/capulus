#ifndef CAPULUS_STATE_H
#define CAPULUS_STATE_H

#include <Arduino.h>
#include <input.h>
#include <display.h>
#include <output.h>

class CAPULUS_STATE {
public:
    CAPULUS_STATE();
    void input(inputData);
    outputData output();
private:
    void persist_load();
    void persist_save();
    int multiplier = 1;
    outputData data;
};

#endif