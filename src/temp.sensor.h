#ifndef CAPULUS_TEMP_SENSOR_H
#define CAPULUS_TEMP_SENSOR_H

#include <max6675.h>

#define ktcSO 15 //D8 
#define ktcCS 13 //D7
#define ktcCLK 12 //D6

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

//If returns incorrect value read with a delay or less frequently
float read_temp(){
    return ktc.readCelsius();
}

#endif