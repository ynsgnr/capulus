#ifndef OUTPUT_H
#define OUTPUT_H

#define SELECT_TEMP 0
#define SELECT_PRESS 1
#define INCREASE_SELECTED(selected) (selected+1)%2

struct outputData{
    char ssid[32] = "";
    char password[32] = "";
    int temp = 90;
    int pressure = 9;
    int selected;
};

#endif