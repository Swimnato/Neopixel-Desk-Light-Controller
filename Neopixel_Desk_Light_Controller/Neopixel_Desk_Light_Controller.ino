#include "configManager.h"
#include "REPL_Client.h"
#include "LightEffects.h"
#include "PhysicalInputs.h"
#include "constants.h"
#include "LightUpdator.h"

uint32_t* lightBuffer;

ConfigManager manager(&lightBuffer);
LightEffects effects(&lightBuffer, &manager);
REPLClient client(&effects, &manager);
LightUpdator updator(&lightBuffer, &manager);


void setup() {

  manager.init();
  client.init();
  effects.init();
  updator.init();
}

void loop() {
  client.processInput();
  effects.runEffect();
  updator.updateLights();
}