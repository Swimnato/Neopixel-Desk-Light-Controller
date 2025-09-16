#include "REPL_Client.h"

void REPLClient::init(){
    Serial.begin(BAUD_RATE);
    delay(100);
    Serial.println(WELCOME_MESSAGE);
    Serial.print(CURSOR);
    currIn.reserve(1000);
}
void REPLClient::processInput(){
    if(Serial.available()){
        while(Serial.available()){
            char toProcess = Serial.read();
            if(toProcess == '\r' || toProcess == '\n'){
                Serial.println();
                runCommand();
                Serial.print(CURSOR);
                currIn.remove(0);
                break;
            }
            if(toProcess != 0x8 && toProcess != 0x7F){
                currIn = currIn + toProcess;
                Serial.print(toProcess);
            }
            else if (currIn.length() != 0){
                Serial.print(toProcess);
                currIn.remove(currIn.length() - 1);
            }
        }
    }
}

void REPLClient::runCommand(){
    if(currIn.isEmpty()){
        return;
    }
    bool inWord = false; 
    int wordStart;
    String command;
    String parameters[MAX_NUM_PARAM];
    uint8_t numberOfParameters = 0;
    for(int i = 0; i < currIn.length(); i++){
        char currChar = currIn[i];
        if((inWord && (isWhitespace(currChar) || i == currIn.length() - 1)) || (!isWhitespace(currChar) && i == currIn.length() - 1)){
            if(command.isEmpty()){
                command = currIn.substring(wordStart, (i == currIn.length() - 1) ? i + 1 :i);
                command.toLowerCase();
            }
            else{
                for(int j = 0; j < MAX_NUM_PARAM; j++){
                    if(parameters[j].isEmpty()){
                        parameters[j] = (i == currIn.length() - 1) ? (inWord ? currIn.substring(wordStart) : currIn.substring(i)) : currIn.substring(wordStart, i);
                        parameters[j].toLowerCase();
                        numberOfParameters++;
                        break;
                    }
                }
            }
            inWord = false;
        }
        else if(!inWord && !isWhitespace(currChar)){
            inWord = true;
            wordStart = i;
        }
    }

    if(command.equals("ls") || command.equals("help")){
        if(parameters[0].equals("modes") || parameters[0].equals("mode")){
            if(isNotRightAmountOfParameters(1, numberOfParameters)){
                return;
            }
            Serial.println(MODES_INTRODUCTION);
            for(int i = START_OF_EFFECTS + 1; i < END_OF_EFFECTS; i++){
                Serial.print("\x1b[33m ");
                Serial.print(i);
                Serial.print("\x1b[0m");
                Serial.print('\t');
                Serial.println(LightEffects::descriptionOfEffects[i]);
            }
        }
        else{
            if(isNotRightAmountOfParameters(0, numberOfParameters)){
                return;
            }
            Serial.println(HELP_LIST);
        }
    }
    else if(command.equals("set")){
        if(parameters[0].equals("color")){
            if(isNotRightAmountOfParameters(4, numberOfParameters)){
                return;
            }
            uint8_t r,g,b;
            r = parameters[1].toInt();
            g = parameters[2].toInt();
            b = parameters[3].toInt();
            effects->setTerminalColor(r,g,b);
        }
        else if(parameters[0].equals("number") && parameters[1].equals("pixels")){
            if(isNotRightAmountOfParameters(3, numberOfParameters)){
                return;
            }
            unsigned int numPixels = parameters[2].toInt();
            if(numPixels <= (unsigned) 0 || numPixels > MAX_NUM_PIXELS){
                Serial.println(ERROR_NUM_PIXELS);
                Serial.println(numPixels);
                #if MULTI_THREADED
                xSemaphoreGive(bufferAndConfigSemaphore);
                #endif
                return;
            }
            if(config->getNumPixels() != numPixels) {
                config->setNumPixels(numPixels);
            }
        }
        else if(parameters[0].equals("saved") && parameters[1].equals("color")){
            if(isNotRightAmountOfParameters(6, numberOfParameters)){
                return;
            }
            bool isMain;
            if(parameters[2].equals("main") || parameters[2].equals("1")){
                isMain = true;
            }
            else if(parameters[2].equals("alt") || parameters[2].equals("2")){
                isMain = false;
            }
            else{
                Serial.println(SPECIFY_MAIN_OR_ALT);
                return;
            }
            uint8_t r,g,b;
            r = parameters[3].toInt();
            g = parameters[4].toInt();
            b = parameters[5].toInt();
            uint32_t color = Adafruit_NeoPixel::Color(r,g,b);
            if(isMain){
                if(config->getSavedColor1() != color){
                    config->setSavedColor1(color);
                }
            }
            else{
                if(config->getSavedColor2() != color){
                    config->setSavedColor2(Adafruit_NeoPixel::Color(r,g,b));
                }
            }
            effects->makeSureUserColorIsVisible(isMain ? 0 : 1);
        } 
        else if(parameters[0].equals("mode")){
            if(isNotRightAmountOfParameters(2, numberOfParameters)){
                return;
            }
            uint8_t mode = parameters[1].toInt();
            if(mode <= START_OF_EFFECTS || mode >= END_OF_EFFECTS){
                Serial.println(ERROR_INVALID_MODE(START_OF_EFFECTS + 1, END_OF_EFFECTS - 1));
                return;
            }
            effects->setMode(mode);
        }
        else if(parameters[0].equals("start") && parameters[1].equals("mode")){
            if(isNotRightAmountOfParameters(3, numberOfParameters)){
                return;
            }
            uint8_t mode = parameters[2].toInt();
            if(mode <= START_OF_EFFECTS || mode >= END_OF_EFFECTS){
                Serial.println(ERROR_INVALID_MODE(START_OF_EFFECTS + 1, END_OF_EFFECTS - 1));
                return;
            }
            if(config->getStartMode() != mode){
                config->setStartMode(mode);
            }
        }
        else if(parameters[0].equals("brightness")){
            if(isNotRightAmountOfParameters(2, numberOfParameters)){
                return;
            }
            uint8_t brightness = (uint8_t) parameters[1].toInt();
            if(brightness == 0){
                Serial.println(ERROR_INVALID_BRIGHTNESS);
                return;
            }
            if(config->getBrightness() != brightness){
                config->setBrightness(brightness);
            }
        }
        else if(parameters[0].equals("number") || parameters[0].equals("num")){
            if(isNotRightAmountOfParameters(3, numberOfParameters)){
                return;
            }
            bool isOn;
            if(parameters[1].equals("on")){
                isOn = true;
            }
            else if(parameters[1].equals("off")){
                isOn = false;
            }
            else{
                Serial.println(SPECIFY_ON_OR_OFF);
                return;
            }
            uint8_t num = parameters[2].toInt();
            if(isOn){
                if(config->getNumOn() != num){
                    config->setNumOn(num);
                }
            }
            else{
                if(config->getNumOff() != num){
                    config->setNumOff(num);
                }
            }
        }
        else{
            Serial.println(ERROR_UNRECOGNIZED_COMMAND);
        }
    }
    else{
        Serial.println(ERROR_UNRECOGNIZED_COMMAND);
    }
}

bool REPLClient::isWhitespace(const char toTest){
    return toTest < 33;
}

bool REPLClient::isNotRightAmountOfParameters(uint8_t numParameters, uint8_t numEntered){
    if(numEntered > numParameters){
        Serial.println(ERROR_TOO_MANY_PARAMETERS);
        return true;
    }
    if(numEntered < numParameters){
        Serial.println(ERROR_TOO_FEW_PARAMETERS);
        return true;
    }
    return false;
}