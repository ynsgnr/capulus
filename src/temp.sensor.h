#include <max6675.h>

#define ktcSO 12
#define ktcCS 13
#define ktcCLK 15

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

float read_temp(){
    return ktc.readCelsius();
}