#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H
#include "arduino.h"
#include "EEPROM.h"
#include "constants.h"

class ConfigManager {
    private:
        int numPixels;
        uint32_t savedColor1;
        uint32_t savedColor2;
        uint32_t** lightBuffer;
        bool useEEPROM;
        uint8_t brightness;
        uint8_t startMode;
        uint8_t numOn;
        uint8_t numOff;
    public:
        ConfigManager(uint32_t** buff): lightBuffer(buff){};
        void init();
        int getNumPixels();
        void setNumPixels(int newNum);
        uint8_t getStartMode();
        void setStartMode(uint8_t mode);
        uint8_t getBrightness();
        void setBrightness(uint8_t amount);
        uint32_t getSavedColor1();
        void setSavedColor1(uint32_t newColor);
        uint32_t getSavedColor2();
        void setSavedColor2(uint32_t newColor);
        uint8_t getNumOn();
        void setNumOn(uint8_t num);
        uint8_t getNumOff();
        void setNumOff(uint8_t num);
};

#include "LightEffects.h"

#endif