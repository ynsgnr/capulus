#include "input.buttons.h"
#include <Arduino.h>

/*
//Display buttons read from analog inputs
//Steam and brew buttons read from digital inputs since they are
//going to be in on state while other buttons are in use
//10k gate resistor:
BUTTON 0 (plus):   543     R:10k       V:1.650 Calculated:512
BUTTON 1 (option):  727     R:5K        V:2.2   Calculated:682
BUTTON 2 (minus):   358     R:20K       V:1.1   Calculated:341
BUTTON 0-1:         817     R:3.33K     V:2.48  Calculated:769
BUTTON 0-2:         649     R:6.66K     V:1.99  Calculated:616
BUTTON 1-2:         779     R:4K        V:2.35  Calculated:731
BUTTON 0-1-2:       848     R:2.85K     V:2.56  Calculated:797

Formula: 3.3/(Total R+R gate)*R gate*1024/3.3
Calculating Total R: (1/R1+1/R2+1/R3...)^-1
*/

#define MINUSVAL 358
#define OPTIONSVAL 727
#define PLUSVAL 543
#define RANGE 50
#define RCALC(value,compare) (value<=compare+RANGE && value>=compare-RANGE)

#define STEAMPIN 3 //RX
#define BREWPIN 1 //TX


CAPULUS_BUTTON_INPUT::CAPULUS_BUTTON_INPUT(){ 
    pinMode(STEAMPIN, INPUT);
    digitalWrite(STEAMPIN, HIGH);
    pinMode(BREWPIN, INPUT);
    digitalWrite(BREWPIN, HIGH);
    result.minus = false;
    result.option = false;
    result.plus = false;
    result.steam = false;
    result.brew = false;
}

inputData CAPULUS_BUTTON_INPUT::read(){
    result.any = false;
    auto buttonValues = analogRead(A0);
    if (RCALC(buttonValues,MINUSVAL)){
        result.minus = true;
        result.option = false;
        result.plus = false;
        result.any = true;
    }else if (RCALC(buttonValues,OPTIONSVAL)){
        result.minus = false;
        result.option = true;
        result.plus = false;
        result.any = true;
    }else if (RCALC(buttonValues,PLUSVAL)){
        result.minus = false;
        result.option = false;
        result.plus = true;
        result.any = true;
    }else{
        result.minus = false;
        result.option = false;
        result.plus = false;
    }
    if (digitalRead(STEAMPIN)==LOW){
        result.any = result.any || !result.steam;
        result.steam = true;
        digitalWrite(STEAMPIN, HIGH);
        digitalWrite(0, HIGH);
    }else{
        result.any = result.any || result.steam;
        result.steam = false;
        digitalWrite(0, LOW);
    }
    if (digitalRead(BREWPIN)==LOW){
        result.any = result.any || !result.brew;
        result.brew = true;
        digitalWrite(BREWPIN, HIGH);
        digitalWrite(0, HIGH);
    }else{
        result.any = result.any || result.brew;
        result.brew = false;
        digitalWrite(0, LOW);
    }
    return result;
}