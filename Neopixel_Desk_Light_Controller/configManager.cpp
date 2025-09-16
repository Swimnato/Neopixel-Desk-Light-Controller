#include "configManager.h"

#define NUM_PIXELS_ADDR 0
#define SAVED_COLOR_1_ADDR 4
#define SAVED_COLOR_2_ADDR 8
#define BRIGHTNESS_ADDR 12
#define START_MODE_ADDR 13
#define NUM_ON_ADDR 14
#define NUM_OFF_ADDR 15

int ConfigManager::getNumPixels(){
    return numPixels;
}
uint32_t ConfigManager::getSavedColor1(){
    return savedColor1;
}
uint32_t ConfigManager::getSavedColor2(){
    return savedColor2;
}

void ConfigManager::init(){
    if (!EEPROM.begin(EEPROM_SIZE)) { 
        useEEPROM = false;
        Serial.println("\x1b[31mfailed to initialize EEPROM\x1b[39m");
        Serial.println("Loading Default Values Instead.");
        numPixels = DEFAULT_NUM_PIXELS;
        savedColor1 = DEFAULT_COLOR_1;
        savedColor2 = DEFAULT_COLOR_2;
        brightness = DEFAULT_BRIGHTNESS;
        startMode = DEFAULT_STARTING_MODE;
        numOn = DEFAULT_NUM_ON;
        numOff = DEFAULT_NUM_OFF;
    }
    else{
        useEEPROM = true;
        numPixels = EEPROM.readInt(NUM_PIXELS_ADDR);
        savedColor1 = EEPROM.readInt(SAVED_COLOR_1_ADDR);
        savedColor2 = EEPROM.readInt(SAVED_COLOR_2_ADDR);
        brightness = EEPROM.read(BRIGHTNESS_ADDR);
        startMode = EEPROM.read(START_MODE_ADDR);
        numOn = EEPROM.read(NUM_ON_ADDR);
        numOff = EEPROM.read(NUM_OFF_ADDR);
        if(numPixels == 0 || numPixels > MAX_NUM_PIXELS || brightness == 0 || startMode <= START_OF_EFFECTS || startMode >= END_OF_EFFECTS){
            Serial.println("\x1b[31mInvalid Values Detected in EEPROM\x1b[39m");
            Serial.println("Loading Default Values, and Writing to Flash");
            numPixels = DEFAULT_NUM_PIXELS;
            savedColor1 = DEFAULT_COLOR_1;
            savedColor2 = DEFAULT_COLOR_2;
            brightness = DEFAULT_BRIGHTNESS;
            startMode = DEFAULT_STARTING_MODE;
            numOn = DEFAULT_NUM_ON;
            numOff = DEFAULT_NUM_OFF;
            EEPROM.writeInt(NUM_PIXELS_ADDR, numPixels);
            EEPROM.writeInt(SAVED_COLOR_1_ADDR, savedColor1);
            EEPROM.writeInt(SAVED_COLOR_2_ADDR, savedColor2);
            EEPROM.write(BRIGHTNESS_ADDR, brightness);
            EEPROM.write(START_MODE_ADDR, startMode);
            EEPROM.write(NUM_ON_ADDR, numOn);
            EEPROM.write(NUM_OFF_ADDR, numOff);
            EEPROM.commit();
        }
    }

    *lightBuffer = new uint32_t[numPixels];
}

void ConfigManager::setNumPixels(int newNum){
    numPixels = newNum;
    if(useEEPROM){
        EEPROM.writeInt(NUM_PIXELS_ADDR, numPixels);
        EEPROM.commit();
    }
    delete *lightBuffer;
    *lightBuffer = new uint32_t[numPixels];
}

void ConfigManager::setSavedColor1(uint32_t newColor){
    savedColor1 = newColor;
    if(useEEPROM){
        EEPROM.writeInt(SAVED_COLOR_1_ADDR, savedColor1);
        EEPROM.commit();
    }
}

void ConfigManager::setSavedColor2(uint32_t newColor){
    savedColor2 = newColor;
    if(useEEPROM){
        EEPROM.writeInt(SAVED_COLOR_2_ADDR, savedColor2);
        EEPROM.commit();
    }
}

uint8_t ConfigManager::getStartMode(){
    return startMode;
}


uint8_t ConfigManager::getBrightness(){
    return brightness;
}


void ConfigManager::setBrightness(uint8_t amount){
    brightness = amount;
    if(useEEPROM){
        EEPROM.write(BRIGHTNESS_ADDR, brightness);
        EEPROM.commit();
    }
}


void ConfigManager::setStartMode(uint8_t mode){
    startMode = mode;
    if(useEEPROM){
        EEPROM.write(START_MODE_ADDR, startMode);
        EEPROM.commit();
    }
}

uint8_t ConfigManager::getNumOn(){
    return numOn;
}

uint8_t ConfigManager::getNumOff(){
    return numOff;
}

void ConfigManager::setNumOn(uint8_t num){
    numOn = num;
    if(useEEPROM){
        EEPROM.write(NUM_ON_ADDR, numOn);
        EEPROM.commit();
    }
}

void ConfigManager::setNumOff(uint8_t num){
    numOff = num;
    if(useEEPROM){
        EEPROM.write(NUM_OFF_ADDR, numOff);
        EEPROM.commit();
    }
}