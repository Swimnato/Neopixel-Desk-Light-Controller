#include "LightUpdator.h"

void LightUpdator::init(){
    strip = new Adafruit_NeoPixel(config->getNumPixels(), NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
    strip->begin();
}

void LightUpdator::updateLights(){
    #if MULTI_THREADED
    if(xSemaphoreTake(bufferAndConfigSemaphore,100)){
    #endif
        if(strip->numPixels() != config->getNumPixels()){
            strip->clear();
            strip->show();
            strip->updateLength(config->getNumPixels());
        }
        for(int i = 0; i < config->getNumPixels(); i++){
            strip->setPixelColor(i, ((*lightBuffer)[i]));
        }
        strip->setBrightness(config->getBrightness());
    #if MULTI_THREADED
        xSemaphoreGive(bufferAndConfigSemaphore);
    }
    #endif
    strip->show();
}