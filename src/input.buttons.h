#ifndef INPUT_BUTTONS_H
#define INPUT_BUTTONS_H

#include <input.h>

class CAPULUS_INPUT{
    public:
        CAPULUS_INPUT();
        inputData read();
    private:
        inputData result;
};

#endif