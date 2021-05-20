#ifndef INPUT_H
#define INPUT_H

struct inputData {
    bool minus;
    bool option;
    bool plus;
    bool steam;
    bool brew;
};

class CAPULUS_INPUT{
    public:
        CAPULUS_INPUT();
        inputData read();
    private:
        inputData result;
};

#endif