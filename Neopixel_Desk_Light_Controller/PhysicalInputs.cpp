#include "PhysicalInputs.h"

void Inputs::init(){
    pinMode(pot1, INPUT);
    pinMode(pot2, INPUT);
    pinMode(button1, INPUT_PULLDOWN);
    pinMode(extDet, INPUT_PULLUP);
}
void Inputs::checkInputs(){ 
    potValue = digitalRead(extDet)? analogRead(pot1) : analogRead(pot2);
    if(digitalRead(button1)){
        if(!buttonPressed){
            buttonPressed = true;
            timeButtonPressed = millis();
        }
    }else{
        buttonPressed = false;
    }
}
bool Inputs::isButtonPressed(){
    return (buttonPressed && (millis() - timeButtonPressed > DEBOUNCE_TIME || millis() < timeButtonPressed));
}
float Inputs::getPotValue(){
    return ((float) potValue) / ((float)MAX_POT_VALUE);
}