#ifndef REPL_CLIENT_H
#define REPL_CLIENT_H

#include "constants.h"
#include "configManager.h"
#include "LightEffects.h"
#include <arduino.h>
#include <Adafruit_NeoPixel.h>

class REPLClient {
    private:

    LightEffects* effects;
    ConfigManager* config;

    String currIn;

    void runCommand();

    bool isWhitespace(const char toTest);

    bool isNotRightAmountOfParameters(uint8_t numParameters, uint8_t numEntered);

    public:

    REPLClient(LightEffects* fx,ConfigManager* cfg) : effects(fx), config(cfg){};

    void init();
    void processInput();

};

#endif