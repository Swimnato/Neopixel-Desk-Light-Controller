#ifndef LIGHT_UPDATOR_H
#define LIGHT_UPDATOR_H

#include "configManager.h"
#include "constants.h"
#include <Adafruit_NeoPixel.h>

class LightUpdator{
    private:

    uint32_t** lightBuffer;
    ConfigManager* config;
    Adafruit_NeoPixel* strip;
    
    public:
    LightUpdator(uint32_t** buff, ConfigManager* cfg): lightBuffer(buff), config(cfg){};

    void init();

    void updateLights();
};

#endif