#include <Arduino.h>

#include "my-serial.h"
#include <Lora.h>
#include <Display.h>
#include <Storage.h>

Lora lora;
Display display;
MySerial mySerial(&lora);
Storage storage;

void settingsUpdatedCallback(LoraSettings settings);
void receiveCallback(String msg);
void loraSetup();

void setup() {
  Serial.begin(115200);

  storage.init();
  display.init();
  loraSetup();
  display.setState(DISPLAY_DASHBOARD);
}

void loraSetup() {
  LoraSettings preferenceSettings = storage.getLoraSettings();

  lora.settings.setSettingsUpdatedCallback(settingsUpdatedCallback);
  lora.setReceiveCallback(receiveCallback);
  lora.init(preferenceSettings);
}

void loop() {
  mySerial.parseSerial();
  lora.check();
  mySerial.checkPending();
}

void settingsUpdatedCallback(LoraSettings loraSettings) {
  display.setDashboardSettings(loraSettings);
  storage.saveLoraSettings(loraSettings);
}

void receiveCallback(String msg) {
  mySerial.parseLoraMessage(msg);
}