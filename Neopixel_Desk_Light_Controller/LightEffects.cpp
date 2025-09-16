#include "LightEffects.h"

void LightEffects::init(){
    inputs.init();
    currMode = config->getStartMode();
}

void LightEffects::runEffect(){
    static bool buttonPressed = false;
    inputs.checkInputs();
    if(inputs.isButtonPressed()){
        if(!buttonPressed){
            buttonPressed = true;
            currMode++;
            if(currMode >= END_OF_EFFECTS || currMode <= START_OF_EFFECTS){
                currMode = START_OF_EFFECTS + 1;
            }
            delayTime = 10;
        }
    } else{
        buttonPressed = false;
    }

    if(readyToCalculateNextFrame()){
        //Serial.println(millis() - lastUpdate);
        lastUpdate = millis();
        switch(currMode){
            case RAINBOW_SOLID:
            rainbowSolid();
            break;
            case RAINBOW_BREATHE:
            rainbowBreathe();
            break;
            case RAINBOW_CHASE:
            rainbowChase();
            break;
            case SOLID_COLOR:
            solidColor(config->getSavedColor1());
            break;
            case COLOR_BREATHE:
            colorBreathe();
            break;
            case COLOR_CHASE:
            colorChase();
            break;
            case SOLID_ALT_COLOR:
            solidColor(config->getSavedColor2());
            break;
            case TWO_COLOR_BREATHE:
            colorBreathe(true);
            break;
            case TWO_COLOR_CHASE:
            colorChase(true);
            break;
            case WHITE_SOLID:
            solidColor(Adafruit_NeoPixel::Color(255,255,255));
            break;
            case TERMINAL_DEFINED_COLOR:
            solidColor(terminalColor);
            break;
            case END_OF_EFFECTS:
            case START_OF_EFFECTS:
            currMode = RAINBOW_BREATHE;
            break;
        }
    }
}

#define WHEEL_ROLLOVER 65536

bool LightEffects::readyToCalculateNextFrame(){
    return (millis() - lastUpdate >= delayTime || millis() < lastUpdate);
}


void LightEffects::rainbowSolid(){
    int numLights = config->getNumPixels();
    (*lightBuffer)[0] = 0;
    uint32_t prevVal = (*lightBuffer)[0];
    (*lightBuffer)[0] = Adafruit_NeoPixel::ColorHSV((*lightBuffer)[0]);
    for(int i = 1; i < numLights; i++){
        (*lightBuffer)[i] = prevVal + (WHEEL_ROLLOVER / config->getNumPixels());
        if((*lightBuffer)[i] >= WHEEL_ROLLOVER){
            (*lightBuffer)[i] = (*lightBuffer)[i] % WHEEL_ROLLOVER;
        }
        prevVal = (*lightBuffer)[i];
        (*lightBuffer)[i] = Adafruit_NeoPixel::ColorHSV((*lightBuffer)[i]);
    }
    lastUpdate = millis();
}

void LightEffects::rainbowBreathe(){
    static uint32_t offset = 0;
    int offsetForNextFrame = 50 + 400 * inputs.getPotValue();
    int numLights = config->getNumPixels();
    (*lightBuffer)[0] = offset;
    uint32_t prevVal = (*lightBuffer)[0];
    (*lightBuffer)[0] = Adafruit_NeoPixel::ColorHSV((*lightBuffer)[0]);
    for(int i = 1; i < numLights; i++){
        (*lightBuffer)[i] = prevVal + (WHEEL_ROLLOVER / 10);
        if((*lightBuffer)[i] >= WHEEL_ROLLOVER){
            (*lightBuffer)[i] = (*lightBuffer)[i] % WHEEL_ROLLOVER;
        }
        prevVal = (*lightBuffer)[i];
        (*lightBuffer)[i] = Adafruit_NeoPixel::ColorHSV((*lightBuffer)[i]);
    }
    lastUpdate = millis();
    offset += offsetForNextFrame;
}

void LightEffects::rainbowChase(){
    static uint32_t offset = 0;
    delayTime = 1000 - (750 * inputs.getPotValue());
    int offsetForNextFrame = (WHEEL_ROLLOVER / 10);
    int numLights = config->getNumPixels();
    (*lightBuffer)[0] = offset;
    uint32_t prevVal = (*lightBuffer)[0];
    (*lightBuffer)[0] = Adafruit_NeoPixel::ColorHSV((*lightBuffer)[0]);
    for(int i = 1; i < numLights; i++){
        (*lightBuffer)[i] = prevVal + (WHEEL_ROLLOVER / 10);
        if((*lightBuffer)[i] >= WHEEL_ROLLOVER){
            (*lightBuffer)[i] = (*lightBuffer)[i] % WHEEL_ROLLOVER;
        }
        prevVal = (*lightBuffer)[i];
        (*lightBuffer)[i] = Adafruit_NeoPixel::ColorHSV((*lightBuffer)[i]);
    }
    lastUpdate = millis();
    offset += offsetForNextFrame;
}

void LightEffects::solidColor(uint32_t color){
    int numLights = config->getNumPixels();
    for(int i = 0; i < numLights; i++){
        (*lightBuffer)[i] = color;
    }
    lastUpdate = millis();
}

void LightEffects::colorBreathe(bool twoColors){
    static uint8_t theta = 0;
    uint8_t addToTheta = 1 + 7 * inputs.getPotValue();
    int numLights = config->getNumPixels();
    uint32_t color = config->getSavedColor1();
    uint8_t r1, r2, g1, g2, b1, b2;
    r1 = (color >> 16) & 0xFF;
    g1 = (color >> 8) & 0xFF;
    b1 = color & 0xFF;
    if(twoColors){
        color = config->getSavedColor2();
        r2 = (color >> 16) & 0xFF;
        g2 = (color >> 8) & 0xFF;
        b2 = color & 0xFF;
    }
    for(int i = 0; i < numLights; i++){
        float multiplier = Adafruit_NeoPixel::sine8(i * 25 + theta) / 255.0;
        if(twoColors){
            float inverseMultiplier = Adafruit_NeoPixel::sine8(i * 25 + theta + 127) / 255.0;
            int r,g,b;
            r = r1 * multiplier + r2 * inverseMultiplier;
            if(r > 255){
                r = 255;
            }
            g = g1 * multiplier + g2 * inverseMultiplier;
            if(g > 255){
                g = 255;
            }
            b = b1 * multiplier + b2 * inverseMultiplier;
            if(b > 255){
                b = 255;
            }
            (*lightBuffer)[i] = Adafruit_NeoPixel::Color(r, g, b);
        }
        else{
            (*lightBuffer)[i] = Adafruit_NeoPixel::Color(r1 * multiplier, g1 * multiplier, b1 * multiplier);
        }
    }
    lastUpdate = millis();
    theta += addToTheta;
}

void LightEffects::colorChase(bool twoColors){
    static char offset = 0;
    delayTime = 1000 - (750 * inputs.getPotValue());
    int numLights = config->getNumPixels();
    uint32_t mainColor = config->getSavedColor1();
    uint32_t altColor = twoColors ? config->getSavedColor2() : Adafruit_NeoPixel::Color(0,0,0);
    for(int i = 0; i < numLights; i++){
        (*lightBuffer)[i] = ((i + offset) % (config->getNumOn() + config->getNumOff())) < config->getNumOn() ? mainColor : altColor;
    }
    offset++;
    if(offset >= config->getNumOn() + config->getNumOff()){
        offset = 0;
    }
    lastUpdate = millis();
}

void LightEffects::setTerminalColor(uint8_t r, uint8_t g, uint8_t b){
    this->setMode(TERMINAL_DEFINED_COLOR);
    terminalColor = Adafruit_NeoPixel::Color(r,g,b);
}

void LightEffects::setMode(uint8_t mode){
    currMode = mode;
    delayTime = 10;
}

const char* LightEffects::descriptionOfEffects[END_OF_EFFECTS] = {
    "START_OF_EFFECTS",
    "\x1b[31mR\x1b[33mA\x1b[32mI\x1b[34mN\x1b[36mB\x1b[35mO\x1b[38;5;129mW\x1b[0m\x1b[32m Solid\x1b[0m - sets the strip to a static rainbow \x1b[23;35m'Taste the rainbow'\x1b[0m",
    "\x1b[31mR\x1b[33mA\x1b[32mI\x1b[34mN\x1b[36mB\x1b[35mO\x1b[38;5;129mW\x1b[0m\x1b[32m Chase\x1b[0m - sets the strip to a moving rainbow \x1b[23;35m'Feel the rainbow'\x1b[0m",
    "\x1b[31mR\x1b[33mA\x1b[32mI\x1b[34mN\x1b[36mB\x1b[35mO\x1b[38;5;129mW\x1b[0m\x1b[32m Breathe\x1b[0m - sets the strip to a rainbow that fades as it moves along \x1b[23;35m'\x1b[1mBECOME\x1b[0m\x1b[23;35m the rainbow'\x1b[0m",
    "\x1b[32mSolid Main Color\x1b[39m - Sets the strip to a solid user-defined color (The first one)",
    "\x1b[32mChase Main Color\x1b[39m - Moves the User-Defined color along a path",
    "\x1b[32mBreathe Main Color\x1b[39m - Fades a user color in and out",
    "\x1b[32mSolid Alt Color\x1b[39m - Sets the strip to a solid user-defined color (The second One)",
    "\x1b[32mTwo Color Chase\x1b[39m - Moves the User-Defined colors along a path",
    "\x1b[32mBreathe Two Colors\x1b[39m - Fades the user colors in and out of eachother",
    "\x1b[32mSolid White\x1b[39m - It's just plain old solid white, what more can you ask for?"
};

const TypeOfEffects LightEffects::effectsWithUserDefinedColor1[] = {
    SOLID_COLOR,
    COLOR_CHASE,
    COLOR_BREATHE,
    TWO_COLOR_CHASE,
    TWO_COLOR_BREATHE,
    END_OF_EFFECTS
};
const TypeOfEffects LightEffects::effectsWithUserDefinedColor2[] = {
    SOLID_ALT_COLOR,
    TWO_COLOR_CHASE,
    TWO_COLOR_BREATHE,
    END_OF_EFFECTS
};
const TypeOfEffects* LightEffects::effectsWithUserDefinedColors[] = {
    effectsWithUserDefinedColor1,
    effectsWithUserDefinedColor2
};


void LightEffects::makeSureUserColorIsVisible(uint8_t color){
    int item;
    for(item = 0; effectsWithUserDefinedColors[color][item] != END_OF_EFFECTS; item++){
        if(currMode == effectsWithUserDefinedColors[color][item]){
            break;
        }
    }
    if(effectsWithUserDefinedColors[color][item] == END_OF_EFFECTS){
        this->setMode(color == 0 ? SOLID_COLOR : SOLID_ALT_COLOR);
    }
}