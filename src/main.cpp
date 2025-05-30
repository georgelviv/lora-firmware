#include <Arduino.h>

#include "my-serial.h"
#include "lora.h"
#include "display.h"
#include "storage.h"

Lora lora;
Display display;
MySerial mySerial(&lora);
Storage storage;

void settingsUpdatedCallback(LoraSettings settings);
void loraSetup();

void setup() {
  Serial.begin(115200);

  display.init();
  loraSetup();
  display.setState(DISPLAY_DASHBOARD);
}

void loraSetup() {
  LoraSettings preferenceSettings = storage.getLoraSettings();

  lora.settings.setSettingsUpdatedCallback(settingsUpdatedCallback);
  lora.init(preferenceSettings);
}

void loop() {
  mySerial.parseSerial();
  lora.check();
}

void settingsUpdatedCallback(LoraSettings loraSettings) {
  display.setDashboardSettings(loraSettings);
}
