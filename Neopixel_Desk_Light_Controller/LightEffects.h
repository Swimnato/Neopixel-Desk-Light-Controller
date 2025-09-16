#ifndef LIGHT_EFFECTS_H
#define LIGHT_EFFECTS_H

#include "configManager.h"
#include "configManager.h"
#include <Adafruit_NeoPixel.h>
#include "PhysicalInputs.h"
#include <arduino.h>

enum TypeOfEffects:uint8_t {
    START_OF_EFFECTS,
    RAINBOW_SOLID,
    RAINBOW_CHASE,
    RAINBOW_BREATHE,
    SOLID_COLOR,
    COLOR_CHASE,
    COLOR_BREATHE,
    SOLID_ALT_COLOR,
    TWO_COLOR_CHASE,
    TWO_COLOR_BREATHE,
    WHITE_SOLID,
    END_OF_EFFECTS,
    TERMINAL_DEFINED_COLOR
};

class LightEffects {
    private:

    uint32_t** lightBuffer;
    unsigned short delayTime = 10;
    ConfigManager* config;
    Inputs inputs;
    unsigned long lastUpdate;
    uint32_t terminalColor;

    static const TypeOfEffects effectsWithUserDefinedColor1[];
    static const TypeOfEffects effectsWithUserDefinedColor2[];
    static const TypeOfEffects* effectsWithUserDefinedColors[];

    uint8_t currMode;

    bool readyToCalculateNextFrame();
    void rainbowSolid();
    void rainbowBreathe();
    void rainbowChase();
    void solidColor(uint32_t color);
    void colorBreathe(bool twoColors = false);
    void colorChase(bool twoColors = false);
    
    public:

    static const char* descriptionOfEffects[END_OF_EFFECTS];

    LightEffects(uint32_t** buff, ConfigManager* cfg): lightBuffer(buff), config(cfg){};
    void init();
    void runEffect();
    void setTerminalColor(uint8_t r, uint8_t g, uint8_t b);
    void setMode(uint8_t mode);
    void makeSureUserColorIsVisible(uint8_t color);
    
};

#endif