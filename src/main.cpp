#include <Arduino.h>

#include "my-serial.h"
#include <Lora.h>
#include <Display.h>
#include <Storage.h>
#include <ApiServer.h>

Lora lora;
Display display;
MySerial mySerial(&lora, &display);
Storage storage;
ApiServer apiServer;

void settingsUpdatedCallback(LoraSettings settings);
void receiveCallback(String msg);
void receiveChunkCallback(int chunk, int totalChunks);
void loraSetup();

void setup() {
  Serial.begin(115200);

  storage.init();
  display.init();
  loraSetup();
  display.setState(DISPLAY_DASHBOARD);
  apiServer.setup();
  display.setWifi(true);
}

void loraSetup() {
  LoraSettings preferenceSettings = storage.getLoraSettings();

  lora.settings.setSettingsUpdatedCallback(settingsUpdatedCallback);
  lora.setReceiveCallback(receiveCallback);
  lora.setReceiveChunkCallback(receiveChunkCallback);
  lora.init(preferenceSettings);
}

void loop() {
  mySerial.parseSerial();
  lora.check();
  mySerial.checkPending();
  display.check();
}

void settingsUpdatedCallback(LoraSettings loraSettings) {
  lora.listen();
  display.setDashboardSettings(loraSettings);
  storage.saveLoraSettings(loraSettings);
}

void receiveCallback(String msg) {

  while (msg.endsWith("_")) {
    msg.remove(msg.length() - 1);
  }

  mySerial.parseLoraMessage(msg);
}

void receiveChunkCallback(int chunk, int totalChunks) {
  mySerial.handleChunkReceived(chunk, totalChunks);
}