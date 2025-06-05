#include "my-serial.h"

MySerial::MySerial(Lora *lora) {
  this->lora = lora;
}

void MySerial::parseSerial() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    String command = getCommand(msg);
    String params = getParams(msg);

    handleSerialMessage(command, params);
  }
}

void MySerial::handleSerialMessage(String command, String params) {
  if (command == "PING") {
    this->sendPing();
  } else if (command == "CONFIG") {
    this->updateSettings(params);
  } else if (command == "CONFIG_SYNC") {
    this->sendConfigSync(params);
  } else {
    Serial.println("Unknown message");
  }
}

void MySerial::sendPing() {
  this->pingStart = millis();
  lora->transmit("PING");
}

void MySerial::parseLoraMessage(String msg) {
  String command = getCommand(msg);
  String params = getParams(msg);

  if (command == "PING") {
    this->sendPingBack();
  } else if (command == "PING_ACK") {
    this->printPingSuccess();
  } else if (command == "CONFIG_SYNC") {
    this->syncConfig(params);
  } else if (command == "CONFIG_SYNC_ACK") {
    this->updateSettingsAndCheck(params);
  } else if (command == "CONFIG_SYNC_CHECK") {
    this->sendConfigCheckAck();
  } else if (command == "CONFIG_SYNC_CHECK_ACK") {
    this->printSuccessConfigUpdate();
  } else {
    Serial.print("Unknown incoming Lora message:");
    Serial.println(msg);
  }
}

void MySerial::updateSettings(String str) {
  String configs[5][2];
  int configsCount = parseParams(str, configs);

  for (int i = 0; i < configsCount; i++) {
    String key = configs[i][0];
    String val = configs[i][1];
    if (key == "FQ") {
      this->lora->settings.updateFrequency(val.toFloat());
    } else if (key == "BW") {
       this->lora->settings.updateFrequency(val.toFloat());
    } else {
      Serial.println("Uknown config to update");
      Serial.println(key);
    }
  }
};

void MySerial::sendPingBack() {
  lora->transmit("PING_ACK");
}

void MySerial::sendConfigSync(String configMsg) {
  this->configSyncStart = millis();
  lora->transmit("CONFIG_SYNC;" + configMsg);
}

void MySerial::syncConfig(String params) {
  lora->setTransmitCallback([this, params]() {
    this->updateSettings(params);
  });

  lora->transmit("CONFIG_SYNC_ACK;" + params);
}

void MySerial::updateSettingsAndCheck(String params) {
  this->updateSettings(params);
  lora->transmit("CONFIG_SYNC_CHECK");
}

void MySerial::printPingSuccess() {
  Serial.println("PING_ACK;" + this->getStatusString(&this->pingStart));
}

void MySerial::sendConfigCheckAck() {
  lora->transmit("CONFIG_SYNC_CHECK_ACK");
}

void MySerial::printSuccessConfigUpdate() {
  Serial.println("CONFIG_SYNC_CHECK_ACK;" +  this->getStatusString(&this->configSyncStart));
}

String MySerial::getStatusString(unsigned long* startTime) {
  unsigned long diff = millis() - *startTime;

  *startTime = 0;
  float rssi = lora->getRSSI();
  float snr = lora->getSNR();
  return "DELAY=" + String(diff) + ",RSSI=" + String(rssi) + ",SNR=" + String(snr);
}