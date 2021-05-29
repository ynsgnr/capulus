#ifndef CAPULUS_TEMP_SENSOR_H
#define CAPULUS_TEMP_SENSOR_H

#include <max6675.h>

#define ktcSO 2 //D4
#define ktcCS 16 //D0
#define ktcCLK 12 //D6

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

//If returns incorrect value read with a delay or less frequently
float read_temp(){
    return ktc.readCelsius();
}

#endif