#ifndef PHYSICAL_INPUTS_H
#define PHYSICAL_INPUTS_H
#include "constants.h"
#include <arduino.h>

class Inputs{
    private:
        bool buttonPressed;
        uint16_t potValue;
        unsigned long timeButtonPressed;
    public:
    
    void init();
    void checkInputs();
    bool isButtonPressed();
    float getPotValue();
};

#endif